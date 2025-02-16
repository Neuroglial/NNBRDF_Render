#include <iostream>
#include "core/platform/renderAPI/RenderAPI.hpp"
#include "core/platform/system/Window.hpp"
#include "core/platform/system/EventManager.hpp"
#include "core/render/Material.hpp"
#include "scene/ImageManager.hpp"
#include "scene/SceneManger.hpp"
#include "scene/ScriptManager.hpp"
#include "scene/ShaderManager.hpp"
#include "scene/MeshManager.hpp"
#include "scene/PipelineManager.hpp"
#include "resource/shaders/uniform/shaders_uniform.hpp"
#include "scene/Camera.hpp"
#include "scene/Light.hpp"

#include "scene/LightManager.hpp"

#include "nlohmann/json.hpp"

#include "editor/UI_utils.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "editor/ImGui.hpp"

#include "scene/CameraUnifrom.hpp"

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

const unsigned int SHADOW_WIDTH = 1024;
const unsigned int SHADOW_HEIGHT = 1024;

void imgui_init(Windows &window);
void imgui_newframe();
void imgui_draw();
void SetDarkThemeColors();
void imgui_destory();

void addPass(Material &mat) {

};

int main()
{
    EventManager event_mgr;
    SceneManager scene_mgr(event_mgr);
    Windows window;

    window.init();
    window.creat_window("NNBRDF_Render", SCR_WIDTH, SCR_HEIGHT, event_mgr);
    RenderAPI::init(GraphicsAPI::OpenGL);

    MeshManager::register_mesh("resource/mesh/cube.obj");
    MeshManager::register_mesh("resource/mesh/quad.obj");

    std::vector<Ref<GameObject>> objects(5);

    Ref<Texture2D> tex_test = RenderAPI::creator<Texture2D>::crt();
    tex_test->init(Tex::REPEAT, Tex::LINEAR);
    tex_test->set_image(utils::read_image(Root_Path + "resource/image/Pixel/white.jpg"));

    Ref<Material> m_cube = std::make_shared<Material>(Root_Path + "resource/shaders/Blinn_Phong_test.glsl", true, Material::Front);
    Ref<Material> m_skybox = std::make_shared<Material>(Root_Path + "resource/shaders/skyBox.glsl", true, Material::Double_Sided);

    auto skybox = scene_mgr.create_Object("Sky Box");
    skybox->add_component<MeshComponent>().m_mesh = MeshManager::get("resource/mesh/cube.obj");
    skybox->get_component<MeshComponent>()->m_castShadow = false;
    skybox->get_component<TransformComponent>()->m_scale = glm::vec3(50, 50, 50);
    skybox->add_component<RendererComponent>().m_materials.push_back(m_skybox);

    auto pointLight = scene_mgr.create_Object("Point Light");
    pointLight->add_component<MeshComponent>().m_mesh = MeshManager::get("resource/mesh/cube.obj");
    pointLight->get_component<MeshComponent>()->m_castShadow = false;
    pointLight->get_component<TransformComponent>()->m_scale = glm::vec3(0.25, 0.25, 0.25);
    pointLight->get_component<TransformComponent>()->m_position = glm::vec3(0, 1, 0);
    pointLight->add_component<RendererComponent>().m_materials.push_back(m_cube);
    pointLight->add_component<PointLightComponent>();

    for (int i = 0; i < objects.size(); ++i)
    {
        objects[i] = scene_mgr.create_Object();

        auto *trans = objects[i]->get_component<TransformComponent>();
        if (trans)
            trans->m_position.x = i * 1.5f;

        auto &mcmp = objects[i]->add_component<MeshComponent>();
        mcmp.m_mesh = MeshManager::get("resource/mesh/cube.obj");
        auto &renders = objects[i]->add_component<RendererComponent>();
        renders.m_materials.push_back(m_cube);
    }

    objects[0]->add_component<ScriptComponent>(std::string("ForwardTest"));

    auto camera_t = scene_mgr.create_Object("Camera");
    camera_t->add_component<CameraComponet>();
    camera_t->add_component<ScriptComponent>(std::string("EditorCamera"));

    Ref<Mesh> cube(RenderAPI::creator<Mesh>::crt());
    cube->as_base_shape(Mesh::Cube);
    Ref<Mesh> quad(RenderAPI::creator<Mesh>::crt());
    quad->as_base_shape(Mesh::Quad);

    auto fun = [&](Event::Event &_event)
    {
        if (auto fr = dynamic_cast<Event::Event_Frame_Resize *>(&_event))
        {
            RenderAPI::viewport(fr->m_width, fr->m_height);
        }
    };
    event_mgr.registerCallback(fun);

    Material mt_light("a_default_vs", "a_light_fs", true, Material::Double_Sided);
    Material mt_depth("a_default_vs", "a_void_fs", true, Material::Double_Sided);
    Material mt_depth_test("b_post_vs", "b_depth_test_fs", false, Material::Double_Sided);
    Material mt_shadow_point(Root_Path + "resource/shaders/PointLightShadowMap.glsl", true, Material::Double_Sided);

    MyCamera camera(75.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, ProjectMode::Persp);
    event_mgr.registerCallback(std::bind(&MyCamera::callback, &camera, std::placeholders::_1));

    auto tex_diffuse = RenderAPI::creator<Texture2D>::crt();
    tex_diffuse->init(Tex::REPEAT, Tex::LINEAR);
    tex_diffuse->set_image(ImageManager::get(Root_Path + "resource/image/container2.png"));
    m_cube->set_param("mt_diffuse", &tex_diffuse);

    auto tex_specular = RenderAPI::creator<Texture2D>::crt();
    tex_specular->init(Tex::REPEAT, Tex::LINEAR);
    tex_specular->set_image(ImageManager::get(Root_Path + "resource/image/container2_specular.png"));
    m_cube->set_param("mt_specular", &tex_specular);

    auto tex_skycube = RenderAPI::creator<TextureCube>::crt();
    tex_skycube->init(Tex::CLAMP, Tex::LINEAR);
    tex_skycube->set_cubemap(Root_Path + "resource/image/skybox/CubeMapTest/CubeMapTest.jpg");
    // mt_skybox.set_param("iChannel0", &tex_skycube);

    RenderAPI::depth_test(true);
    // RenderAPI::face_culling(true);

    float mt_shininess = 32.0f;

    imgui_init(window);

    auto tex_depth = RenderAPI::creator<Texture2D>::crt();
    tex_depth->init(Tex::CLAMP, Tex::LINEAR, Tex::Depth);
    auto tex_shadow_cube = RenderAPI::creator<TextureCube>::crt();
    tex_shadow_cube->init(Tex::CLAMP, Tex::LINEAR, Tex::Depth);
    tex_shadow_cube->resize(1024, 1024);

    auto tex_color = RenderAPI::creator<Texture2D>::crt();
    tex_color->init(Tex::REPEAT, Tex::LINEAR, Tex::RGB);

    auto fb_depth = RenderAPI::creator<FrameBuffer>::crt();
    fb_depth->init(SCR_WIDTH, SCR_HEIGHT);
    fb_depth->attach(tex_depth, 0);
    fb_depth->attach(tex_color, 0);
    m_skybox->set_param("iChannel0", &tex_shadow_cube);

    mt_depth_test.set_param("iChannel0", &tex_color);
    mt_depth_test.set_param("iChannel1", &tex_depth);

    bool depth = false;

    float aspect = (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT;
    float near = 0.1f;
    float far = 20.0f;
    glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far);

    scene_mgr.Start();

    while (!window.shouldClose())
    {
        imgui_newframe();
        ImGui::Begin("Controller");
        UI::RenderSceneTree(scene_mgr.get_root());
        ImGui::End();

        camera.tick(0.01f);
        scene_mgr.Update(0.01f);

        m_cube->set_param("mt_shininess", &mt_shininess);
        ub_camera_data.projection = camera.m_camera.get_projection();
        ub_camera_data.view = glm::inverse(camera.get_model());
        ub_camera_data.viewPos = camera.get_position();

        if (auto pt_data = pointLight->get_component<PointLightComponent>())
        {
            LightManager::RenderPointLightShadowMap(tex_shadow_cube, &pt_data->m_data, &scene_mgr);
        }

        auto wnsize = window.get_window_size();
        RenderAPI::viewport(wnsize.x, wnsize.y);
        fb_depth->resize(wnsize.x, wnsize.y);
        fb_depth->bind(glm::vec4(1, 0, 0, 1));

        scene_mgr.render_mesh();

        fb_depth->unbind();

        CameraUniform::bind(camera_t->get_component<CameraComponet>());

        float depthf = depth;
        quad->draw(mt_depth_test);

        imgui_draw();
        window.swapBuffer();
    }

    imgui_destory();

    return 0;
}

void imgui_destory()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
}

void imgui_draw()
{
    // Rendering ImGui
    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

void imgui_init(Windows &window)
{
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    // io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           // Enable Docking
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         // Enable Multi-Viewport / Platform Windows
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoTaskBarIcons;
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsNoMerge;

    ImFont *pFont = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\segoeui.ttf", 18.0f);
    io.FontDefault = io.Fonts->Fonts.back();

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsClassic();

    // When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
    ImGuiStyle &style = ImGui::GetStyle();
    // if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    // {
    //     style.WindowRounding = 0.0f;
    //     style.Colors[ImGuiCol_WindowBg].w = 1.0f;
    // }

    SetDarkThemeColors();

    // Setup Platform/Renderer bindings
    ImGui_ImplGlfw_InitForOpenGL(window.get_window(), true);
    ImGui_ImplOpenGL3_Init("#version 410");
}

void SetDarkThemeColors()
{
    auto &colors = ImGui::GetStyle().Colors;
    colors[ImGuiCol_WindowBg] = ImVec4{0.1f, 0.105f, 0.11f, 1.0f};

    // Headers
    colors[ImGuiCol_Header] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
    colors[ImGuiCol_HeaderHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
    colors[ImGuiCol_HeaderActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

    // Buttons
    colors[ImGuiCol_Button] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
    colors[ImGuiCol_ButtonHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
    colors[ImGuiCol_ButtonActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

    // Frame BG
    colors[ImGuiCol_FrameBg] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};
    colors[ImGuiCol_FrameBgHovered] = ImVec4{0.3f, 0.305f, 0.31f, 1.0f};
    colors[ImGuiCol_FrameBgActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

    // Tabs
    colors[ImGuiCol_Tab] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    colors[ImGuiCol_TabHovered] = ImVec4{0.38f, 0.3805f, 0.381f, 1.0f};
    colors[ImGuiCol_TabActive] = ImVec4{0.28f, 0.2805f, 0.281f, 1.0f};
    colors[ImGuiCol_TabUnfocused] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    colors[ImGuiCol_TabUnfocusedActive] = ImVec4{0.2f, 0.205f, 0.21f, 1.0f};

    // Title
    colors[ImGuiCol_TitleBg] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    colors[ImGuiCol_TitleBgActive] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};
    colors[ImGuiCol_TitleBgCollapsed] = ImVec4{0.15f, 0.1505f, 0.151f, 1.0f};

    // Resize Grip
    colors[ImGuiCol_ResizeGrip] = ImVec4(0.91f, 0.91f, 0.91f, 0.25f);
    colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.81f, 0.81f, 0.81f, 0.67f);
    colors[ImGuiCol_ResizeGripActive] = ImVec4(0.46f, 0.46f, 0.46f, 0.95f);

    // Scrollbar
    colors[ImGuiCol_ScrollbarBg] = ImVec4(0.02f, 0.02f, 0.02f, 0.53f);
    colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.31f, 0.31f, 0.31f, 1.0f);
    colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.41f, 0.41f, 0.41f, 1.0f);
    colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.51f, 0.51f, 0.51f, 1.0f);

    // Check Mark
    colors[ImGuiCol_CheckMark] = ImVec4(0.94f, 0.94f, 0.94f, 1.0f);

    // Slider
    colors[ImGuiCol_SliderGrab] = ImVec4(0.51f, 0.51f, 0.51f, 0.7f);
    colors[ImGuiCol_SliderGrabActive] = ImVec4(0.66f, 0.66f, 0.66f, 1.0f);
}

void imgui_newframe()
{
    // Start a new ImGui frame
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplGlfw_NewFrame();
    ImGui::NewFrame();
}