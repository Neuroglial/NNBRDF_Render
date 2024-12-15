#include <iostream>
#include "core/platform/renderAPI/RenderAPI.hpp"
#include "core/platform/system/Window.hpp"
#include "core/platform/system/EventManager.hpp"
#include "core/render/Material.hpp"
#include "scene/ImageManager.hpp"
#include "scene/ShaderManager.hpp"
#include "scene/PipelineManager.hpp"
#include "resource/shaders/shaders_uniform.hpp"
#include "scene/Camera.hpp"
#include "scene/Light.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

int main()
{

    EventManager event_mgr;
    Windows window;
    window.init();
    window.creat_window("NNBRDF_Render", SCR_WIDTH, SCR_HEIGHT, event_mgr);
    RenderAPI::init(GraphicsAPI::OpenGL);

    std::vector<Ref<Mesh>> meshes;
    utils::loadModel(Root_Path + "resource/mesh/cube.obj", meshes);

    auto cube = meshes[0];
    // cube->as_base_shape(Mesh::Cube);

    auto fun = [&](Event::Event &_event)
    {
        if (auto fr = dynamic_cast<Event::Event_Frame_Resize *>(&_event))
        {
            RenderAPI::viewport(fr->m_width, fr->m_height);
        }
    };
    event_mgr.registerCallback(fun);

    Material mt_phong("a_default_vs", "a_phong_BRDF_fs");
    Material mt_light("a_default_vs", "a_light_fs");
    Material mt_skybox("a_default_vs", "a_skybox_fs");

    MyCamera camera(45, (float)SCR_WIDTH / (float)SCR_HEIGHT, ProjectMode::Persp);
    event_mgr.registerCallback(std::bind(&MyCamera::callback, &camera, std::placeholders::_1));

    auto tex_diffuse = RenderAPI::creator<Texture2D>::crt();
    tex_diffuse->init(Tex_WarppingMode::REPEAT, Tex_FilteringMode::LINEAR);
    tex_diffuse->set_image(ImageManager::get_hdr(Root_Path + "resource/image/container2.png"));
    mt_phong.set_param("mt_diffuse", &tex_diffuse);

    auto tex_specular = RenderAPI::creator<Texture2D>::crt();
    tex_specular->init(Tex_WarppingMode::REPEAT, Tex_FilteringMode::LINEAR);
    tex_specular->set_image(ImageManager::get(Root_Path + "resource/image/container2_specular.png"));
    mt_phong.set_param("mt_specular", &tex_specular);

    auto tex_skybox = RenderAPI::creator<Texture2D>::crt();
    tex_skybox->init(Tex_WarppingMode::REPEAT, Tex_FilteringMode::LINEAR);
    tex_skybox->set_image(ImageManager::get_hdr(Root_Path + "resource/hdr/wildflower_field_4k.hdr"));
    mt_skybox.set_param("iChannel0", &tex_skybox);

    auto ub_camera = RenderAPI::creator<UniformBuffer>::crt();
    ub_camera->reset(sizeof(ub_camera_data), 1);

    auto ub_lights = RenderAPI::creator<UniformBuffer>::crt();
    ub_lights->reset(sizeof(ub_lights_data), 2);

    glm::vec3 cb_pos{1, 0, -1};
    glm::vec3 cb_scl{0.5, 0.5, 0.5};
    glm::vec3 cb_rot{0, 0, 0};

    glm::vec3 sb_scl{20, 20, 20};

    PointLight point_light;

    glm::vec3 lt_pos{0, 1, -1};
    glm::vec3 lt_scl{0.2, 0.2, 0.2};
    glm::vec3 lt_rot{0, 0, 0};

    RenderAPI::depth_test(true);
    // RenderAPI::face_culling(true);

    // 初始化ImGui上下文
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;

    // 设置ImGui样式
    ImGui::StyleColorsDark();

    // 初始化ImGui绑定
    ImGui_ImplGlfw_InitForOpenGL(window.get_window(), true);
    ImGui_ImplOpenGL3_Init("#version 330");

    float mt_shininess = 32.0f;

    while (!window.shouldClose())
    {
        RenderAPI::clear();

        camera.tick(0.01f);
        point_light.buffer_update(lt_pos, glm::vec3(0));
        mt_phong.set_param("mt_shininess", &mt_shininess);

        ub_camera_data.projection = camera.m_camera.get_projection();
        ub_camera_data.view = glm::inverse(camera.get_model());
        ub_camera_data.view_pos = camera.get_position();

        ub_camera->set_data(0, sizeof(ub_camera_data), &ub_camera_data);
        ub_lights->set_data(0, sizeof(ub_lights_data), &ub_lights_data);

        auto cube_model = utils::get_model(cb_pos, cb_scl, cb_rot);
        mt_phong.set_param("model", &cube_model);
        cube->draw(mt_phong);

        auto light_model = utils::get_model(lt_pos, lt_scl, lt_rot);
        mt_light.set_param("model", &light_model);
        cube->draw(mt_light);

        auto sb_model = glm::scale(glm::mat4(1), sb_scl);
        mt_skybox.set_param("model", &sb_model);
        cube->draw(mt_skybox);

        // 开始新的ImGui帧
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // 创建窗口和控件
        ImGui::Begin("Controller");
        ImGui::DragFloat("constant", &point_light.m_constant);
        ImGui::DragFloat("linear", &point_light.m_linear);
        ImGui::DragFloat("quadratic", &point_light.m_quadratic);
        ImGui::DragFloat("shininess", &mt_shininess);
        ImGui::End();

        // 渲染ImGui
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        window.swapBuffer();
    }

    return 0;
}