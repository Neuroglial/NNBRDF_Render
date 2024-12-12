#include <glad/glad.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/platform/system/Window.hpp"
#include "core/platform/system/EventManager.hpp"

#include "core/platform/renderAPI/RenderAPI.hpp"

#include "core/render/Material.hpp"

#include "scene/ImageManager.hpp"
#include "scene/ShaderManager.hpp"
#include "scene/PipelineManager.hpp"

#include "scene/Camera.hpp"

struct ShaderBase
{
    alignas(4) float iTime;
    alignas(4) float iTimeDelta;
    alignas(4) int iFrame;
    alignas(4) float iFrameRate;
    alignas(16) glm::vec4 iMouse;
    alignas(16) glm::vec3 iResolution;
};

struct Light
{
    alignas(16) glm::vec3 light_pos;
    alignas(16) glm::vec3 light_color;
};

struct Lights_Uniform_Struct
{
    int num;
    alignas(16) Light lg[10];
};

struct Camera_Uniform_Struct
{
    glm::vec3 view_pos;
    alignas(16) glm::mat4 projection;
    alignas(16) glm::mat4 view;
};

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
    //cube->as_base_shape(Mesh::Cube);

    auto fun = [&](Event::Event &_event)
    {
        if (auto fr = dynamic_cast<Event::Event_Frame_Resize *>(&_event))
        {
            glViewport(0, 0, fr->m_width, fr->m_height);
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

    Camera_Uniform_Struct camera_st;
    auto ub_camera = RenderAPI::creator<UniformBuffer>::crt();
    ub_camera->reset(sizeof(camera_st), 0);

    Lights_Uniform_Struct lights_st;
    auto ub_lights = RenderAPI::creator<UniformBuffer>::crt();
    ub_lights->reset(sizeof(lights_st), 1);

    lights_st.num = 1;

    glm::vec3 cb_pos{1, 0, -1};
    glm::vec3 cb_scl{0.5, 0.5, 0.5};
    glm::vec3 cb_rot{0, 0, 0};

    glm::vec3 lt_pos{0, 1, -1};
    glm::vec3 lt_scl{0.2, 0.2, 0.2};
    glm::vec3 lt_rot{0, 0, 0};
    glm::vec3 lt_col{1, 1, 1};

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);

    while (!window.shouldClose())
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        lights_st.lg[0].light_color = lt_col;
        lights_st.lg[0].light_pos = lt_pos;

        camera.tick(0.01f);

        camera_st.projection = camera.m_camera.get_projection();
        camera_st.view = glm::inverse(camera.get_model());
        camera_st.view_pos = camera.get_position();

        ub_camera->set_data(0,sizeof(camera_st),&camera_st);
        ub_lights->set_data(0,sizeof(lights_st),&lights_st);

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