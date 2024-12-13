#include <iostream>
#include "core/platform/renderAPI/RenderAPI.hpp"
#include "core/platform/system/Window.hpp"
#include "core/platform/system/EventManager.hpp"
#include "core/render/Material.hpp"
#include "scene/ImageManager.hpp"
#include "scene/ShaderManager.hpp"
#include "scene/PipelineManager.hpp"
#include "source/shaders/shaders_uniform.hpp"
#include "scene/Camera.hpp"

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
    utils::loadModel(Root_Path + "source/mesh/cube.obj", meshes);

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

    Material mt_default("a_default_vs", "a_default_fs");
    Material mt_light("a_default_vs", "a_light_fs");
    MyCamera camera(45, (float)SCR_WIDTH / (float)SCR_HEIGHT, ProjectMode::Persp);
    event_mgr.registerCallback(std::bind(&MyCamera::callback, &camera, std::placeholders::_1));

    auto texture1 = RenderAPI::creator<Texture2D>::crt();
    texture1->set_sample(Tex_WarppingMode::REPEAT, Tex_FilteringMode::Mipmap);
    texture1->set_image(ImageManager::get(Root_Path + "source/image/container.jpg"));
    mt_default.set_param("texture1", &texture1);

    auto texture2 = RenderAPI::creator<Texture2D>::crt();
    texture2->set_sample(Tex_WarppingMode::REPEAT, Tex_FilteringMode::Mipmap);
    texture2->set_image(ImageManager::get(Root_Path + "source/image/awesomeface.png"));
    mt_default.set_param("texture2", &texture2);

    auto ub_camera = RenderAPI::creator<UniformBuffer>::crt();
    ub_camera->reset(sizeof(ub_camera_data), 1);

    auto ub_lights = RenderAPI::creator<UniformBuffer>::crt();
    ub_lights->reset(sizeof(ub_light_data), 2);

    ub_light_data.pl_num = 1;

    glm::vec3 cb_pos{1, 0, -1};
    glm::vec3 cb_scl{0.5, 0.5, 0.5};
    glm::vec3 cb_rot{0, 0, 0};

    glm::vec3 lt_pos{0, 1, -1};
    glm::vec3 lt_scl{0.2, 0.2, 0.2};
    glm::vec3 lt_rot{0, 0, 0};
    glm::vec3 lt_col{1, 1, 1};

    RenderAPI::depth_test(true);
    RenderAPI::face_culling(true);

    while (!window.shouldClose())
    {
        RenderAPI::clear();

        ub_light_data.pl[0].color = lt_col;
        ub_light_data.pl[0].position = lt_pos;

        camera.tick(0.01f);

        ub_camera_data.projection = camera.m_camera.get_projection();
        ub_camera_data.view = glm::inverse(camera.get_model());
        ub_camera_data.view_pos = camera.get_position();

        ub_camera->set_data(0, sizeof(ub_camera_data), &ub_camera_data);
        ub_lights->set_data(0, sizeof(ub_light_data), &ub_light_data);

        auto cube_model = utils::get_model(cb_pos, cb_scl, cb_rot);
        mt_default.set_param("model", &cube_model);
        cube->draw(mt_default);

        auto light_model = utils::get_model(lt_pos, lt_scl, lt_rot);
        mt_light.set_param("model", &light_model);
        cube->draw(mt_light);

        window.swapBuffer();
    }

    return 0;
}