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

#include "scene/LightUniform.hpp"

#include "nlohmann/json.hpp"

#include "editor/UI_utils.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "editor/ImGui.hpp"

#include "scene/CameraUnifrom.hpp"

#include "scene/BaseInfoUniform.hpp"

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

void imgui_renderMartial(Material *mat);

void DrawPass(const std::function<void()> &renderer, FrameBuffer *frameBuffer = nullptr, glm::vec4 clear = glm::vec4(-1.0f))
{
    if (frameBuffer)
    {
        if (clear.x >= 0.0f)
            frameBuffer->bind(clear);
        else
            frameBuffer->bind();

        if (frameBuffer->get_size() != RenderAPI::get_viewportSize())
            RenderAPI::viewport(frameBuffer->get_size());
    }
    else if (RenderAPI::get_frameBufferSize() != RenderAPI::get_viewportSize())
        RenderAPI::viewport(RenderAPI::get_frameBufferSize());

    BaseInfoUniform::bind();

    renderer();

    if (frameBuffer)
        frameBuffer->unbind();
};

void DrawPass(Material *mat, FrameBuffer *frameBuffer = nullptr, glm::vec4 clear = glm::vec4(-1.0f))
{
    Assert(mat);

    static Ref<Mesh> quad;
    if (quad == nullptr)
    {
        quad = RenderAPI::creator<Mesh>::crt();
        quad->as_base_shape(Mesh::Quad);
    }

    auto renderer = [&mat]()
    { quad->draw(*mat); };

    DrawPass(renderer, frameBuffer, clear);
};

void RenderHDRandBloom(Ref<Texture2D> texture, FrameBuffer *output = nullptr)
{
    Assert(texture);

    auto texSize = texture->get_size();

    static Ref<Material> m_BloomPassA;
    static Ref<Material> m_BloomPassB;

    static Ref<Texture2D> frameMidColor;
    static Ref<FrameBuffer> frameMid;

    if (m_BloomPassA == nullptr)
    {
        m_BloomPassA = std::make_shared<Material>(Root_Path + "resource/shaders/BloomPassA.glsl", false, Material::Double_Sided);
        m_BloomPassB = std::make_shared<Material>(Root_Path + "resource/shaders/BloomPassB.glsl", false, Material::Double_Sided);

        frameMidColor = RenderAPI::creator<Texture2D>::crt();
        frameMidColor->init(Tex::REPEAT, Tex::LINEAR, Tex::RGB | Tex::Bit16);

        frameMid = RenderAPI::creator<FrameBuffer>::crt();
        frameMid->init(texSize.x, texSize.y);
        frameMid->attach(frameMidColor, 0);
    }

    if (frameMid->get_size() != texSize)
        frameMid->resize(texSize);

    m_BloomPassA->set_param("iChannel0", &texture);
    DrawPass(m_BloomPassA.get(), frameMid.get());

    m_BloomPassB->set_param("iChannel0", &texture);
    m_BloomPassB->set_param("iChannel1", &frameMidColor);
    DrawPass(m_BloomPassB.get(), output);
}

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
    MeshManager::register_mesh("resource/mesh/geosphere.obj");

    std::vector<Ref<GameObject>> objects(5);

    Ref<Texture2D> tex_test = RenderAPI::creator<Texture2D>::crt();
    tex_test->init(Tex::REPEAT, Tex::LINEAR);
    tex_test->set_image(utils::read_image(Root_Path + "resource/image/Pixel/white.jpg"));

    Ref<Material> m_BlinnPhong = std::make_shared<Material>(Root_Path + "resource/shaders/Blinn_Phong.glsl", true, Material::Front);
    Ref<Material> m_Light_White[4];
    Ref<Material> m_skybox = std::make_shared<Material>(Root_Path + "resource/shaders/skyBox.glsl", true, Material::Double_Sided);
    Ref<Material> m_PBR = std::make_shared<Material>(Root_Path + "resource/shaders/GGX_PBR.glsl", true, Material::Front);
    Ref<Material> m_Bloom = std::make_shared<Material>(Root_Path + "resource/shaders/Bloom.glsl", false, Material::Double_Sided);

    float bloom_Threshold = 0.0f;
    m_Bloom->set_param("Threshold", &bloom_Threshold);

    float bloom_Intensity = 0.6f;
    m_Bloom->set_param("Intensity", &bloom_Intensity);

    float bloom_BlurSize = 6.0f;
    m_Bloom->set_param("BlurSize", &bloom_BlurSize);

    auto tex_diffuse = RenderAPI::creator<Texture2D>::crt();
    tex_diffuse->init(Tex::REPEAT, Tex::LINEAR);
    tex_diffuse->set_image(ImageManager::get(Root_Path + "resource/image/container2.png"));
    m_BlinnPhong->set_param("mt_diffuse", &tex_diffuse);

    auto tex_specular = RenderAPI::creator<Texture2D>::crt();
    tex_specular->init(Tex::REPEAT, Tex::LINEAR);
    tex_specular->set_image(ImageManager::get(Root_Path + "resource/image/container2_specular.png"));
    m_BlinnPhong->set_param("mt_specular", &tex_specular);

    auto pbr_albedo = RenderAPI::creator<Texture2D>::crt();
    pbr_albedo->init(Tex::REPEAT, Tex::LINEAR);
    pbr_albedo->set_image(ImageManager::get(Root_Path + "resource/image/pbr/rusted_iron/albedo.png"));
    m_PBR->set_param("albedoMap", &pbr_albedo);

    auto pbr_normal = RenderAPI::creator<Texture2D>::crt();
    pbr_normal->init(Tex::REPEAT, Tex::LINEAR);
    pbr_normal->set_image(ImageManager::get(Root_Path + "resource/image/pbr/rusted_iron/normal.png"));
    m_PBR->set_param("normalMap", &pbr_normal);

    auto pbr_metallic = RenderAPI::creator<Texture2D>::crt();
    pbr_metallic->init(Tex::REPEAT, Tex::LINEAR);
    pbr_metallic->set_image(ImageManager::get(Root_Path + "resource/image/pbr/rusted_iron/metallic.png"));
    m_PBR->set_param("metallicMap", &pbr_metallic);

    auto pbr_roughness = RenderAPI::creator<Texture2D>::crt();
    pbr_roughness->init(Tex::REPEAT, Tex::LINEAR);
    pbr_roughness->set_image(ImageManager::get(Root_Path + "resource/image/pbr/rusted_iron/roughness.png"));
    m_PBR->set_param("roughnessMap", &pbr_roughness);

    auto pbr_ao = RenderAPI::creator<Texture2D>::crt();
    pbr_ao->init(Tex::REPEAT, Tex::LINEAR);
    pbr_ao->set_image(ImageManager::get(Root_Path + "resource/image/pbr/rusted_iron/ao.png"));
    m_PBR->set_param("aoMap", &pbr_ao);

    auto skybox = scene_mgr.create_Object("Sky Box");
    skybox->add_component<MeshComponent>().m_mesh = MeshManager::get("resource/mesh/cube.obj");
    skybox->get_component<MeshComponent>()->m_castShadow = false;
    skybox->get_component<TransformComponent>()->m_scale = glm::vec3(50, 50, 50);
    skybox->add_component<RendererComponent>().m_materials.push_back(m_skybox);

    Ref<GameObject> pointLight[4];

    auto lightColor = glm::vec3(0.8f);
    for (int i = 0; i < 4; ++i)
    {
        m_Light_White[i] = std::make_shared<Material>(Root_Path + "resource/shaders/LightColor.glsl", true, Material::Front);
        m_Light_White[i]->set_param("lightColor", &lightColor);

        pointLight[i] = scene_mgr.create_Object("Point Light_" + std::to_string(i));
        pointLight[i]->add_component<MeshComponent>().m_mesh = MeshManager::get("resource/mesh/cube.obj");
        pointLight[i]->add_component<ScriptComponent>(std::string("LightBox"));
        pointLight[i]->get_component<MeshComponent>()->m_castShadow = false;
        pointLight[i]->get_component<TransformComponent>()->m_scale = glm::vec3(0.25, 0.25, 0.25);
        pointLight[i]->get_component<TransformComponent>()->m_position = glm::vec3(0.75 * i, 1, 0);
        pointLight[i]->add_component<RendererComponent>().m_materials.push_back(m_Light_White[i]);
        pointLight[i]->add_component<PointLightComponent>();
    }

    for (int i = 0; i < objects.size(); ++i)
    {
        objects[i] = scene_mgr.create_Object("Cube_" + std::to_string(i));

        auto *trans = objects[i]->get_component<TransformComponent>();
        if (trans)
        {
            trans->m_position.x = i * 1.5f;
            trans->m_scale = glm::vec3(0.5f);
        }
        auto &mcmp = objects[i]->add_component<MeshComponent>();
        mcmp.m_mesh = MeshManager::get("resource/mesh/geosphere.obj");
        auto &renders = objects[i]->add_component<RendererComponent>();
        renders.m_materials.push_back(m_PBR);
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

    Material mt_depth("a_default_vs", "a_void_fs", true, Material::Double_Sided);
    Material mt_depth_color_Changer("b_post_vs", "b_depth_test_fs", false, Material::Double_Sided);
    Material mt_shadow_point(Root_Path + "resource/shaders/PointLightShadowMap.glsl", true, Material::Double_Sided);

    MyCamera camera(75.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, ProjectMode::Persp);
    event_mgr.registerCallback(std::bind(&MyCamera::callback, &camera, std::placeholders::_1));

    auto tex_skycube = RenderAPI::creator<TextureCube>::crt();
    tex_skycube->init(Tex::CLAMP, Tex::LINEAR);
    tex_skycube->set_image(utils::get_color_Image(glm::vec4(0.25f), 3));
    m_skybox->set_param("iChannel0", &tex_skycube);

    RenderAPI::depth_test(true);
    // RenderAPI::face_culling(true);

    float mt_shininess = 32.0f;
    m_BlinnPhong->set_param("mt_shininess", &mt_shininess);

    imgui_init(window);

    auto frameDepthA = RenderAPI::creator<Texture2D>::crt();
    frameDepthA->init(Tex::CLAMP, Tex::LINEAR, Tex::Depth);

    auto frameColorA = RenderAPI::creator<Texture2D>::crt();
    frameColorA->init(Tex::REPEAT, Tex::LINEAR, Tex::RGB | Tex::Bit16);

    auto frameBufferA = RenderAPI::creator<FrameBuffer>::crt();
    frameBufferA->init(SCR_WIDTH, SCR_HEIGHT);
    frameBufferA->attach(frameDepthA, 0);
    frameBufferA->attach(frameColorA, 0);

    mt_depth_color_Changer.set_param("iChannel0", &frameColorA);
    m_Bloom->set_param("iChannel0", &frameColorA);
    mt_depth_color_Changer.set_param("iChannel1", &frameDepthA);

    bool depth = false;

    float aspect = (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT;
    float near = 0.1f;
    float far = 20.0f;
    glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far);

    scene_mgr.Start();

    LightManager::init();

    while (!window.shouldClose())
    {
        // imgui-------------------------------
        imgui_newframe();
        ImGui::Begin("Controller");
        UI::PushID(m_Bloom.get());
        ImGui::Checkbox("Show Depth Map", &depth);
        ImGui::Text("Bloom Post:");
        imgui_renderMartial(m_Bloom.get());
        UI::PopID();
        ImGui::End();
        UI::RenderSceneTree(scene_mgr.get_root());

        // update------------------------------
        camera.tick(0.01f);
        scene_mgr.Update(0.01f);
        CameraUniform::bind(camera_t->get_component<CameraComponet>());

        auto wnsize = window.get_window_size();
        if (frameBufferA->get_size() != wnsize)
            frameBufferA->resize(wnsize.x, wnsize.y);

        float depthValue = depth;
        mt_depth_color_Changer.set_param("depth", &depthValue);

        // render------------------------------
        DrawPass([&scene_mgr]()
                 { scene_mgr.render_mesh(); }, frameBufferA.get(), glm::vec4(0, 0, 0, 1));

        // RenderHDRandBloom(frameColorA);

        // DrawPass(&mt_depth_color_Changer);

        DrawPass(m_Bloom.get());

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

void imgui_renderMartial(Material *mat)
{
    auto list = mat->get_params_list();

    for (auto &i : list->m_param_list)
    {
        switch (i.second.m_type)
        {
        case ShaderParam_Type::Int:
        {
            if (!i.second.m_value_ptr)
                break;

            int *value = (int *)i.second.m_value_ptr;
            UI::Property(i.first.c_str(), *value);
            break;
        }

        case ShaderParam_Type::Float:
        {
            if (!i.second.m_value_ptr)
                break;

            float *value = (float *)i.second.m_value_ptr;
            UI::Property(i.first.c_str(), *value);
            break;
        }

        case ShaderParam_Type::Vec2:
        {
            if (!i.second.m_value_ptr)
                break;

            glm::vec2 *value = (glm::vec2 *)i.second.m_value_ptr;
            UI::Property(i.first.c_str(), *value);
            break;
        }

        case ShaderParam_Type::Vec3:
        {
            if (!i.second.m_value_ptr)
                break;

            glm::vec3 *value = (glm::vec3 *)i.second.m_value_ptr;
            UI::PropertyColor(i.first.c_str(), *value);
            break;
        }

        case ShaderParam_Type::Vec4:
        {
            if (!i.second.m_value_ptr)
                break;

            glm::vec4 *value = (glm::vec4 *)i.second.m_value_ptr;
            UI::Property(i.first.c_str(), *value);
            break;
        }

        default:
            break;
        }
    }
}