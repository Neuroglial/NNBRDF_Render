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
void imgui_actor_info(const std::string &label, Actor &act);
void imgui_material_info(const std::string &label, Material &mat);
void imgui_point_light_info(const std::string &label, PointLight &point_light);
void SetDarkThemeColors();
void imgui_destory();

void addPass(Material &mat) {

};

int main()
{
    glm::vec3 rot(15,25,35);
    auto quat = utils::to_quat(rot);
    rot = utils::to_euler(quat);


    EventManager event_mgr;
    SceneManager scene_mgr;
    Windows window;

    window.init();
    window.creat_window("NNBRDF_Render", SCR_WIDTH, SCR_HEIGHT, event_mgr);
    RenderAPI::init(GraphicsAPI::OpenGL);

    MeshManager::register_mesh("resource/mesh/cube.obj");
    MeshManager::register_mesh("resource/mesh/quad.obj");

    std::vector<Ref<GameObject>> objects(5);

    Ref<Material> m_cube = std::make_shared<Material>(Root_Path + "resource/shaders/Blinn_Phong_test.glsl", true, Material::Front);
    Material &mt_phong = *m_cube.get();

    for (int i = 0; i < objects.size(); ++i)
    {
        objects[i] = scene_mgr.create_Object();

        auto *trans = objects[i]->get_component<TransformComponent>();
        if (trans)
            trans->m_pos.x = i * 3.0f;

        auto &mcmp = objects[i]->add_component<MeshComponent>();
        mcmp.m_mesh = MeshManager::get("resource/mesh/cube.obj");
        auto &renders = objects[i]->add_component<RendererComponent>();
        renders.m_materials.push_back(m_cube);
    }

    auto camera_t = scene_mgr.create_Object("Camera");
    camera_t->add_component<CameraComponet>();

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
    Material mt_skybox("a_default_vs", "a_skybox_cubemap_fs", true, Material::Double_Sided);
    Material mt_shadow_point("c_point_shadow_vs", "c_point_shadow_gs", "c_point_shadow_fs", true, Material::Double_Sided);

    MyCamera camera(75.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, ProjectMode::Persp);
    event_mgr.registerCallback(std::bind(&MyCamera::callback, &camera, std::placeholders::_1));

    auto tex_diffuse = RenderAPI::creator<Texture2D>::crt();
    tex_diffuse->init(Tex::REPEAT, Tex::LINEAR);
    tex_diffuse->set_image(ImageManager::get(Root_Path + "resource/image/container2.png"));
    mt_phong.set_param("mt_diffuse", &tex_diffuse);

    auto tex_specular = RenderAPI::creator<Texture2D>::crt();
    tex_specular->init(Tex::REPEAT, Tex::LINEAR);
    tex_specular->set_image(ImageManager::get(Root_Path + "resource/image/container2_specular.png"));
    mt_phong.set_param("mt_specular", &tex_specular);

    auto tex_skycube = RenderAPI::creator<TextureCube>::crt();
    tex_skycube->init(Tex::CLAMP, Tex::LINEAR);
    tex_skycube->set_cubemap(Root_Path + "resource/image/skybox/CubeMapTest/CubeMapTest.jpg");
    mt_skybox.set_param("iChannel0", &tex_skycube);

    auto ub_camera = RenderAPI::creator<UniformBuffer>::crt();
    ub_camera->reset(sizeof(ub_camera_data), 1);

    auto ub_lights = RenderAPI::creator<UniformBuffer>::crt();
    ub_lights->reset(sizeof(ub_lights_data), 2);

    struct Cube
    {
        glm::vec3 pos;
        glm::vec3 scal;
        glm::vec3 rota;
    };

    Cube skybox = {{0, 0, 0}, {20, 20, 20}, {0, 0, 0}};
    Cube light = {{0, 1, -1}, {0.05, 0.05, 0.05}, {0, 0, 0}};

    PointLight point_light;

    RenderAPI::depth_test(true);
    // RenderAPI::face_culling(true);

    float mt_shininess = 32.0f;

    imgui_init(window);

    auto tex_depth = RenderAPI::creator<Texture2D>::crt();
    tex_depth->init(Tex::CLAMP, Tex::LINEAR, Tex::Depth);

    auto tex_shadow_cube = RenderAPI::creator<TextureCube>::crt();
    tex_shadow_cube->init(Tex::CLAMP, Tex::LINEAR, Tex::Depth);

    auto tex_color = RenderAPI::creator<Texture2D>::crt();
    tex_color->init(Tex::REPEAT, Tex::LINEAR, Tex::RGB);

    auto fb_depth = RenderAPI::creator<FrameBuffer>::crt();
    fb_depth->init(SCR_WIDTH, SCR_HEIGHT);
    fb_depth->attach(tex_depth, 0);
    fb_depth->attach(tex_color, 0);

    auto fb_shadow_map = RenderAPI::creator<FrameBuffer>::crt();
    fb_shadow_map->init(SHADOW_WIDTH, SHADOW_HEIGHT);
    fb_shadow_map->attach(tex_shadow_cube, 0);
    // mt_skybox.set_param("iChannel0", &tex_shadow_cube);
    //mt_phong.set_param("depthMap[0]", &tex_shadow_cube);

    mt_depth_test.set_param("iChannel0", &tex_color);
    mt_depth_test.set_param("iChannel1", &tex_depth);

    bool depth = false;

    float aspect = (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT;
    float near = 0.1f;
    float far = 20.0f;
    glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far);

    scene_mgr.Start();


    PointLight_t pt;
    pt.color = glm::vec3(0.8f);
    pt.intensity = 12.0f;
    pt.ptMapIndex = -1;
    pt.radius = 10.0f;
    

    while (!window.shouldClose())
    {
        LightManager::Clear();
        pt.position = light.pos;
        LightManager::AddLight(pt);
        LightManager::UpdataBuffer();

        auto cameraBuffer = camera_t->get_component<CameraComponet>();
        cameraBuffer->m_view = glm::inverse(camera.get_model());
        cameraBuffer->m_proj = camera.m_camera.get_projection();
        camera_t->get_component<TransformComponent>()->m_pos = camera.get_position();
        CameraUniform::bind(camera_t->get_component<CameraComponet>());
        CameraUniform::UpdataBuffer();


        camera.tick(0.01f);
        scene_mgr.Update(0.01f);
        light.pos = camera.get_position() + camera.get_forward() * 3.0f;

        point_light.buffer_update(light.pos, glm::vec3(0));
        mt_phong.set_param("mt_shininess", &mt_shininess);

        ub_camera_data.projection = camera.m_camera.get_projection();
        ub_camera_data.view = glm::inverse(camera.get_model());
        ub_camera_data.viewPos = camera.get_position();

        //ub_camera->set_data(0, sizeof(ub_camera_data), &ub_camera_data);
        ub_lights->set_data(0, sizeof(ub_lights_data), &ub_lights_data);

        fb_shadow_map->bind(glm::vec4(0, 0, 0, 1));
        auto sdsize = fb_shadow_map->get_size();
        RenderAPI::viewport(sdsize.x, sdsize.y);
        glm::mat4 shadowMat_0 = shadowProj * glm::lookAt(light.pos, light.pos + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));
        glm::mat4 shadowMat_1 = shadowProj * glm::lookAt(light.pos, light.pos + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));
        glm::mat4 shadowMat_2 = shadowProj * glm::lookAt(light.pos, light.pos + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
        glm::mat4 shadowMat_3 = shadowProj * glm::lookAt(light.pos, light.pos + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0));
        glm::mat4 shadowMat_4 = shadowProj * glm::lookAt(light.pos, light.pos + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0));
        glm::mat4 shadowMat_5 = shadowProj * glm::lookAt(light.pos, light.pos + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0));

        mt_shadow_point.set_param("lightPos", &light.pos);
        mt_shadow_point.set_param("far_plane", &far);
        mt_shadow_point.set_param("shadowMat_0", &shadowMat_0);
        mt_shadow_point.set_param("shadowMat_1", &shadowMat_1);
        mt_shadow_point.set_param("shadowMat_2", &shadowMat_2);
        mt_shadow_point.set_param("shadowMat_3", &shadowMat_3);
        mt_shadow_point.set_param("shadowMat_4", &shadowMat_4);
        mt_shadow_point.set_param("shadowMat_5", &shadowMat_5);

        scene_mgr.render_mesh(&mt_shadow_point);

        auto wnsize = window.get_window_size();
        RenderAPI::viewport(wnsize.x, wnsize.y);
        fb_depth->resize(wnsize.x, wnsize.y);
        fb_depth->bind(glm::vec4(1, 0, 0, 1));

        //mt_phong.set_param("lightPos", &light.pos);
        //mt_phong.set_param("far_plane", &far);

        scene_mgr.render_mesh(&mt_phong);

        auto light_model = utils::get_model(light.pos, light.scal, light.rota);
        cube->draw(mt_light, light_model);

        auto sky_model = utils::get_model(skybox.pos, skybox.scal, skybox.rota);
        cube->draw(mt_skybox, sky_model);

        fb_depth->unbind();

        float depthf = depth;
        quad->draw(mt_depth_test);

        imgui_newframe();

        // Creating windows and controls
        ImGui::Begin("Controller");

        UI::RenderSceneTree(scene_mgr.get_root());

        imgui_point_light_info("Point Light", point_light);
        imgui_actor_info("Camera", camera);
        ImGui::End();

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

void imgui_material_info(const std::string &label, Material &mat)
{
    auto &list = mat.get_params_list();

    ImGui::Text("Material %s Information: ", label.c_str());
}

void imgui_actor_info(const std::string &label, Actor &act)
{
    auto &pos = act.get_position();
    auto &scl = act.get_scale();
    auto &rot = act.get_rotation();
    ImGui::Text("%s Information: ", label.c_str());
    ImGui::Text("  Position : %8.1f,%8.1f,%8.1f", pos.x, pos.y, pos.z);
    ImGui::Text("  Scale    : %8.1f,%8.1f,%8.1f", scl.x, scl.y, scl.z);
    ImGui::Text("  Rotation : %8.1f,%8.1f,%8.1f", rot.x, rot.y, rot.z);
}

void imgui_point_light_info(const std::string &label, PointLight &point_light)
{
    ImGui::Text("%s Information: ", label.c_str());
    ImGui::DragFloat("constant", &point_light.m_constant);
    ImGui::DragFloat("linear", &point_light.m_linear);
    ImGui::DragFloat("quadratic", &point_light.m_quadratic);
    ImGui::DragFloat("strength", &point_light.m_strength);
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