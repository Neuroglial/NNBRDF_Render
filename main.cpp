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

#include "core/platform/renderAPI/OpenGL/FrameBuffer_GL.hpp"

#include "core/render/Material.hpp"

#include "scene/ImageManager.hpp"
#include "scene/ShaderManager.hpp"
#include "scene/PipelineManager.hpp"

#include "scene/Camera.hpp"

struct Light
{
    alignas(16) glm::vec3 light_pos;
    alignas(16) glm::vec3 light_color;
};

struct Lights
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

void input_callback(Event::Event &_event);

// settings
const unsigned int SCR_WIDTH = 1920;
const unsigned int SCR_HEIGHT = 1080;

int main()
{
    EventManager event_mgr;
    event_mgr.registerCallback(input_callback);

    Windows window;
    window.init();
    window.creat_window("NNBRDF_Render", SCR_WIDTH, SCR_HEIGHT, event_mgr);
    RenderAPI::init(GraphicsAPI::OpenGL);

    // 初始化Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();

    // 初始化ImGui的GLFW和OpenGL3绑定
    ImGui_ImplGlfw_InitForOpenGL(window.get_window(), true);
    ImGui_ImplOpenGL3_Init("#version 130");

    auto cube_pipe = RenderAPI::creator<Pipeline>::crt();
    cube_pipe->attach_shader(ShaderManager::get("a_default_vs"));
    cube_pipe->attach_shader(ShaderManager::get("a_default_fs"));

    auto light_pipe = RenderAPI::creator<Pipeline>::crt();
    light_pipe->attach_shader(ShaderManager::get("a_default_vs"));
    light_pipe->attach_shader(ShaderManager::get("a_light_fs"));

    Material mt_cube(cube_pipe);
    Material mt_light(light_pipe);

    auto cube = RenderAPI::creator<Mesh>::crt();
    cube->as_base_shape(Mesh::Cube);

    MyCamera camera(45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, ProjectMode::Persp);
    event_mgr.registerCallback(std::bind(&Actor::callback, &camera, std::placeholders::_1));

    auto texture1 = RenderAPI::creator<Texture2D>::crt();
    texture1->set_image(ImageManager::get(Root_Path + "source/image/container.jpg"));
    texture1->set_sample(Tex_WarppingMode::REPEAT, Tex_FilteringMode::Mipmap);
    mt_cube.set_param("texture1", &texture1);

    auto texture2 = RenderAPI::creator<Texture2D>::crt();
    texture2->set_image(ImageManager::get(Root_Path + "source/image/awesomeface.png"));
    texture2->set_sample(Tex_WarppingMode::REPEAT, Tex_FilteringMode::Mipmap);
    mt_cube.set_param("texture2", &texture2);

    glm::vec3 pos_cube(0, 0, -2);
    glm::vec3 scale_cube(1);
    glm::vec3 rotate_cube(0);

    glm::vec3 scale_light(0.2);

    auto ub_camera = RenderAPI::creator<UniformBuffer>::crt();
    ub_camera->reset(144, 0);
    auto ub_lights =RenderAPI::creator<UniformBuffer>::crt();
    ub_lights->reset(336, 1);

    Lights lights;
    lights.num = 1;
    lights.lg[0].light_color = glm::vec3(1);
    lights.lg[0].light_pos = glm::vec3(0.8, 1.2, 0);

    Camera_Uniform_Struct camera_us;

    while (!window.shouldClose())
    {
        glEnable(GL_DEPTH_TEST);
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // 开始新的ImGui帧
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        static bool win = true;

        if (ImGui::Begin("Controller", &win))
        {

            ImGui::Bullet();
            ImGui::Text("Light:");
            ImGui::BeginChild(106, ImVec2(20, 94), false);
            ImGui::EndChild();
            ImGui::SameLine();
            ImGui::BeginChild(108, ImVec2(205, 150), false);

            ImGui::Text("Color:");
            ImGui::PushItemWidth(200);
            ImGui::ColorEdit3("##1", &lights.lg[0].light_color.x);
            ImGui::PopItemWidth();

            ImGui::Text("Position:");
            ImGui::PushItemWidth(200);
            ImGui::InputFloat3("##2", &lights.lg[0].light_pos.x);
            ImGui::PopItemWidth();
            ImGui::EndChild();
        }
        ImGui::End();

        // 渲染
        ImGui::Render();

        camera.tick(0.01);

        camera_us.view_pos = camera.get_position();
        camera_us.projection = camera.m_camera.get_projection();
        camera_us.view = glm::inverse(camera.get_model());

        ub_camera->set_data(0, 144, &camera_us);
        ub_lights->set_data(0, 44, &lights.num);

        glm::mat4 model = glm::translate(glm::mat4(1), pos_cube);
        model = glm::scale(model, scale_cube);
        model = glm::rotate_slow(model, rotate_cube.y, glm::vec3(0, 1, 0));
        model = glm::rotate_slow(model, rotate_cube.x, glm::vec3(1, 0, 0));
        model = glm::rotate_slow(model, rotate_cube.z, glm::vec3(0, 0, 1));
        mt_cube.set_param("model", &model);

        cube->draw(mt_cube);

        model = glm::translate(glm::mat4(1), lights.lg[0].light_pos);
        model = glm::scale(model, scale_light);
        mt_light.set_param("model", &model);

        cube->draw(mt_light);

        // std::cout<<std::to_string(camera.get_position())<<std::endl;

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        window.swapBuffer();
    }

    return 0;
}

void input_callback(Event::Event &_event)
{
    std::cout << _event.get_event() << std::endl;

    if (auto fr = dynamic_cast<Event::Event_Frame_Resize *>(&_event))
    {
        glViewport(0, 0, fr->m_width, fr->m_height);
    }
    else if (auto kb = dynamic_cast<Event::Event_Keyboard *>(&_event))
    {
    }
    else if (auto mm = dynamic_cast<Event::Event_Mouse_Move *>(&_event))
    {
    }
}