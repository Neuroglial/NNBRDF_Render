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

#include "editor/UI_utils.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

#include "editor/ImGui.hpp"

#include "scene/CameraUnifrom.hpp"

#include "scene/BaseInfoUniform.hpp"

#include "scene/Serialize.hpp"
#include "scene/MaterialManager.hpp"

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

        // if (frameBuffer->get_size() != RenderAPI::get_viewportSize())
        //     RenderAPI::viewport(frameBuffer->get_size());
    }
    // else if (RenderAPI::get_frameBufferSize() != RenderAPI::get_viewportSize())
    //     RenderAPI::viewport(RenderAPI::get_frameBufferSize());

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

Ref<FrameBuffer> createFrameBuffer(int width, int height, Tex::WarppingMode warp, Tex::FilteringMode filter, uint32_t channels)
{
    auto frameColor = RenderAPI::creator<Texture2D>::crt();
    frameColor->init(warp, filter, channels);
    auto frameDepth = RenderAPI::creator<Texture2D>::crt();
    frameDepth->init(warp, filter, Tex::Depth);

    auto frameBuffer = RenderAPI::creator<FrameBuffer>::crt();
    frameBuffer->init(width, height);
    frameBuffer->attach(frameColor, 0);
    frameBuffer->attach(frameDepth, 1);

    return frameBuffer;
}

void BloomPass(Ref<FrameBuffer> &fragin, FrameBuffer *fragout = nullptr)
{
    static bool once = true;

    static Ref<Material> m_BloomA;
    static Ref<Material> m_BloomB;
    static Ref<Material> m_BloomC;
    static Ref<Material> m_BloomD;
    static Ref<Material> m_BloomL;

    static Ref<FrameBuffer> frameBuffer;
    static Ref<FrameBuffer> frameBufferA;
    static Ref<FrameBuffer> frameBufferB;
    static Ref<FrameBuffer> frameBufferC;

    if (once)
    {
        once = false;

        m_BloomA = std::make_shared<Material>("resource/shaders/BloomA.glsl", false, Material::Double_Sided);
        m_BloomB = std::make_shared<Material>("resource/shaders/BloomB.glsl", false, Material::Double_Sided);
        m_BloomC = std::make_shared<Material>("resource/shaders/BloomC.glsl", false, Material::Double_Sided);
        m_BloomD = std::make_shared<Material>("resource/shaders/BloomD.glsl", false, Material::Double_Sided);
        m_BloomL = std::make_shared<Material>("resource/shaders/HightLightFliter.glsl", false, Material::Double_Sided);

        frameBufferA = createFrameBuffer(SCR_WIDTH, SCR_HEIGHT, Tex::CLAMP, Tex::LINEAR, Tex::RGB | Tex::Bit16);
        frameBufferB = createFrameBuffer(SCR_WIDTH, SCR_HEIGHT, Tex::CLAMP, Tex::LINEAR, Tex::RGB | Tex::Bit16);
        frameBufferC = createFrameBuffer(SCR_WIDTH, SCR_HEIGHT, Tex::CLAMP, Tex::LINEAR, Tex::RGB | Tex::Bit16);

        m_BloomB->set_param("iChannel0", &frameBufferA->get());
        m_BloomC->set_param("iChannel0", &frameBufferB->get());
        m_BloomD->set_param("iChannel1", &frameBufferC->get());

        float gamma = 0.45;
        m_BloomD->set_param("gamma", &gamma);
        float Bloom_Intensity = 0.2;
        m_BloomD->set_param("Bloom_Intensity", &Bloom_Intensity);
    }

    ImGui::Begin("PostPrecess");
    UI::PushID(m_BloomA.get());
    ImGui::Text("BloomD");
    UI::DrawParams(m_BloomD->m_Params->m_list);
    UI::PopID();
    ImGui::End();

    if (frameBufferA->get_size() != RenderAPI::get_viewportSize())
    {
        frameBufferA->resize(RenderAPI::get_viewportSize());
        frameBufferB->resize(RenderAPI::get_viewportSize());
        frameBufferC->resize(RenderAPI::get_viewportSize());
    }

    m_BloomA->set_param("iChannel0", &fragin->get());
    m_BloomD->set_param("iChannel0", &fragin->get());

    DrawPass(m_BloomA.get(), frameBufferA.get());
    DrawPass(m_BloomB.get(), frameBufferB.get());
    DrawPass(m_BloomC.get(), frameBufferC.get());
    if (fragout)
    {
        DrawPass(m_BloomD.get(), fragout);
    }
    else
    {
        DrawPass(m_BloomD.get());
    }
}

struct testSerialize
{
    PARAMS;
    PM(PM_Int, testInt);
    PM(PM_Vec3, testVec3);
    PM(PM_Mat4, testMat4);
};

int main()
{
    EventManager event_mgr;
    SceneManager scene_mgr(event_mgr);
    Windows window;

    window.init();
    window.creat_window("NNBRDF_Render", SCR_WIDTH, SCR_HEIGHT, event_mgr);
    RenderAPI::init(GraphicsAPI::OpenGL);
    RenderAPI::viewport(SCR_WIDTH, SCR_HEIGHT);

    // MeshManager::register_mesh("resource/mesh/cube.obj");
    // MeshManager::register_mesh("resource/mesh/quad.obj");
    // MeshManager::register_mesh("resource/mesh/geosphere.obj");

    // std::vector<Ref<GameObject>> objects(5);

    // Ref<Material> m_BlinnPhong = std::make_shared<Material>("resource/shaders/Blinn_Phong.glsl", true, Material::Front);
    // Ref<Material> m_Light_White[4];
    // Ref<Material> m_skybox = MaterialManager::get("resource/material/SkyBox.mat");

    // auto m_Light_White = std::make_shared<Material>("resource/shaders/LightColor.glsl", true, Material::Front);
    // auto lightColor = glm::vec3(0.8f);
    // m_Light_White->set_param("lightColor", &lightColor);
    // to_file(m_Light_White, "resource/material/LightColor.mat");

    // auto tex_diffuse = RenderAPI::creator<Texture2D>::crt();
    // tex_diffuse->init(Tex::REPEAT, Tex::LINEAR);
    // tex_diffuse->set_image(ImageManager::get("resource/image/container2.png"));
    // m_BlinnPhong->set_param("mt_diffuse", &tex_diffuse);

    // auto tex_specular = RenderAPI::creator<Texture2D>::crt();
    // tex_specular->init(Tex::REPEAT, Tex::LINEAR);
    // tex_specular->set_image(ImageManager::get("resource/image/container2_specular.png"));
    // m_BlinnPhong->set_param("mt_specular", &tex_specular);

    // auto m_PBR = MaterialManager::get("resource/material/PBR.mat");

    // auto skybox = scene_mgr.create_Object();
    // skybox->add_component<MeshComponent>().m_mesh = MeshManager::get("resource/mesh/cube.obj");
    // skybox->get_component<MeshComponent>()->m_castShadow = false;
    // skybox->get_component<TransformComponent>()->m_scale = glm::vec3(50, 50, 50);
    // skybox->add_component<RendererComponent>().m_materials.push_back(m_skybox);
    // skybox->add_component<ScriptComponent>().set_script("TestScript");

    // to_file(skybox, "resource/prefab/skybox.pref");
    // from_json_ptr(from_file_json("resource/prefab/skybox.pref"), skybox);

    // Ref<GameObject> pointLight[4];

    //
    // for (int i = 0; i < 4; ++i)
    // {
    //     m_Light_White[i] = std::make_shared<Material>("resource/shaders/LightColor.glsl", true, Material::Front);
    //     m_Light_White[i]->set_param("lightColor", &lightColor);

    //     pointLight[i] = scene_mgr.create_Object("Point Light_" + std::to_string(i));
    //     pointLight[i]->add_component<MeshComponent>().m_mesh = MeshManager::get("resource/mesh/geosphere.obj");
    //     pointLight[i]->add_component<ScriptComponent>().set_script("LightBox");
    //     pointLight[i]->get_component<MeshComponent>()->m_castShadow = false;
    //     pointLight[i]->get_component<TransformComponent>()->m_scale = glm::vec3(0.25, 0.25, 0.25);
    //     pointLight[i]->get_component<TransformComponent>()->m_position = glm::vec3(0.75 * i, 1, 0);
    //     pointLight[i]->add_component<RendererComponent>().m_materials.push_back(m_Light_White[i]);
    //     pointLight[i]->add_component<PointLightComponent>();
    // }

    // for (int i = 0; i < objects.size(); ++i)
    // {
    //     objects[i] = scene_mgr.create_Object("Cube_" + std::to_string(i));

    //     auto *trans = objects[i]->get_component<TransformComponent>();
    //     if (trans)
    //     {
    //         trans->m_position.x = i * 1.5f;
    //         trans->m_scale = glm::vec3(0.5f);
    //     }
    //     auto &mcmp = objects[i]->add_component<MeshComponent>();
    //     mcmp.m_mesh = MeshManager::get("resource/mesh/geosphere.obj");
    //     auto &renders = objects[i]->add_component<RendererComponent>();
    //     renders.m_materials.push_back(m_PBR);
    // }

    // objects[0]->add_component<ScriptComponent>().set_script("ForwardTest");

        // Ref<Mesh> cube(RenderAPI::creator<Mesh>::crt());
    // cube->as_base_shape(Mesh::Cube);
    // Ref<Mesh> quad(RenderAPI::creator<Mesh>::crt());
    // quad->as_base_shape(Mesh::Quad);

    auto fun = [&](Event::Event &_event)
    {
        if (auto fr = dynamic_cast<Event::Event_Frame_Resize *>(&_event))
        {
            RenderAPI::viewport(fr->m_width, fr->m_height);
        }
    };
    event_mgr.registerCallback(fun);

    Material mt_depth_color_Changer("resource/shaders/DepthColorChanger.glsl", false, Material::Double_Sided);
    // Material mt_shadow_point("resource/shaders/PointLightShadowMap.glsl", true, Material::Double_Sided);

    // MyCamera camera(75.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, ProjectMode::Persp);
    // event_mgr.registerCallback(std::bind(&MyCamera::callback, &camera, std::placeholders::_1));

    auto tex_skycube = RenderAPI::creator<TextureCube>::crt();
    tex_skycube->init(Tex::CLAMP, Tex::LINEAR);
    tex_skycube->set_cubemap("resource/image/skybox/CubeMapTest/CubeMapTest.jpg", true);
    // tex_skycube->set_image(utils::get_color_Image(glm::vec4(0.25f), 3));

    LightManager::init();

    RenderAPI::depth_test(true);
    // RenderAPI::face_culling(true);

    // float mt_shininess = 32.0f;
    // m_BlinnPhong->set_param("mt_shininess", &mt_shininess);

    imgui_init(window);

    auto frameBuffer = createFrameBuffer(SCR_WIDTH, SCR_HEIGHT, Tex::CLAMP, Tex::LINEAR, Tex::RGB | Tex::Bit16);

    mt_depth_color_Changer.set_param("iChannel0", &frameBuffer->get());
    //  m_BloomL->set_param("iChannel0", &frameBuffer->get());

    bool depth = false;

    float aspect = (float)SHADOW_WIDTH / (float)SHADOW_HEIGHT;
    float near = 0.1f;
    float far = 20.0f;
    glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), aspect, near, far);

    scene_mgr.loadScene("resource/scene/test1.scene");
    scene_mgr.Start();
    while (!window.shouldClose())
    {
        // imgui-------------------------------
        imgui_newframe();
        UI::RenderSceneTree(scene_mgr.get_root());

        // update------------------------------
        // camera.tick(0.01f);
        scene_mgr.Update(0.01f);

        float depthValue = depth;
        mt_depth_color_Changer.set_param("depth", &depthValue);

        if (frameBuffer->get_size() != window.get_window_size())
            frameBuffer->resize(window.get_window_size());

        if (RenderAPI::get_viewportSize() != window.get_window_size())
            RenderAPI::viewport(window.get_window_size());

        // render------------------------------
        DrawPass([&scene_mgr]()
                 { scene_mgr.render_mesh(); }, frameBuffer.get(), glm::vec4(1, 1, 1, 1));

        BloomPass(frameBuffer);

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
    RendererComponent::DrawParams(mat->get_params().get()->m_list);
}