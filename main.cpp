#include <glad/glad.h>
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <GLFW/glfw3.h>
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "core/platform/renderAPI/OpenGL/Shader_GL.hpp"
#include "core/platform/renderAPI/Shader.hpp"
#include "scene/ShaderManager.hpp"
#include "shaders/shaders.hpp"
#include "core/platform/system/Window.hpp"
#include "core/platform/system/EventManager.hpp"
#include "scene/ImageManager.hpp"
#include "core/platform/renderAPI/OpenGL/Texture_GL.hpp"
#include "core/platform/renderAPI/OpenGL/ArrayBuffer_GL.hpp"
#include "core/platform/renderAPI/OpenGL/Mesh_GL.hpp"
#include "scene/Camera.hpp"
#include "core/platform/renderAPI/OpenGL/UniformBuffer_GL.hpp"

struct Light{
    alignas(16) glm::vec3 light_pos;
    alignas(16) glm::vec3 light_color;
};

struct Lights{
    int num;
    alignas(16) Light lg[10];
};

void input_callback(Event::Event &_event);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    EventManager event_mgr;
    event_mgr.registerCallback(input_callback);

    Windows window;
    window.init();
    window.creat_window("NNBRDF_Render", SCR_WIDTH, SCR_HEIGHT, event_mgr);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    // 初始化Dear ImGui
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void)io;
    ImGui::StyleColorsDark();

    // 初始化ImGui的GLFW和OpenGL3绑定
    ImGui_ImplGlfw_InitForOpenGL(window.get_window(), true);
    ImGui_ImplOpenGL3_Init("#version 130");

    Shader::Pipline_GL cube_pipe;
    cube_pipe.attach_shader(ShaderManager::get("../source/shaders/cube.vs"));
    cube_pipe.attach_shader(ShaderManager::get("../source/shaders/cube.fs"));

    Shader::Pipline_GL light_pipe;
    light_pipe.attach_shader(ShaderManager::get("../source/shaders/cube.vs"));
    light_pipe.attach_shader(ShaderManager::get("../source/shaders/light.fs"));

    Mesh_GL mesh(Mesh::Cube);
    MyCamera camera(45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, ProjectMode::Persp);
    event_mgr.registerCallback(std::bind(Actor::callback, &camera, std::placeholders::_1));

    CubeVS::Parameters pm_cube_vs;
    CubeFS::Parameters pm_cube_fs;
    LightFS::Parameters pm_light_fs;

    pm_cube_fs.texture1 = std::make_shared<Texture::Texture2D_GL>(
        Texture::REPEAT, Texture::Mipmap, ImageManager::get("../source/image/container.jpg"));
    pm_cube_fs.texture2 = std::make_shared<Texture::Texture2D_GL>(
        Texture::REPEAT, Texture::Mipmap, ImageManager::get("../source/image/awesomeface.png"));

    glm::vec3 pos_cube(0, 0, -2);
    glm::vec3 scale_cube(1);
    glm::vec3 rotate_cube(0);

    glm::vec3 scale_light(0.2);

    UniformBuffer_GL ub_camera(12, 0);
    UniformBuffer_GL ub_lights(336,1);

    Lights lights;
    lights.num = 1;
    lights.lg[0].light_color = glm::vec3(1);
    lights.lg[0].light_pos = glm::vec3(0.8,1.2,0);

    while (!window.shouldClose())
    {
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
            ImGui::BeginChild(108, ImVec2(205, 94), false);
            ImGui::Text("Color:");
            ImGui::PushItemWidth(200);
            ImGui::ColorEdit3("##1", &lights.lg[0].light_color.x);
            ImGui::PopItemWidth();
            ImGui::Text("Position:");
            ImGui::PushItemWidth(200);
            static float vec4a113[4] = {0.10f, 0.20f, 0.30f, 0.44f};
            ImGui::InputFloat3("##2", &lights.lg[0].light_pos.x);
            ImGui::PopItemWidth();
            ImGui::EndChild();
        }
        ImGui::End();

        // 渲染
        ImGui::Render();

        camera.tick(0.01);

        ub_camera.set_data(0, 12, &camera.get_position());
        ub_lights.set_data(0,44,&lights.num);

        pm_cube_vs.projection = camera.m_camera.get_projection();
        pm_cube_vs.view = glm::inverse(camera.get_model());

        pm_cube_vs.model = glm::translate(glm::mat4(1), pos_cube);
        pm_cube_vs.model = glm::scale((glm::mat4)pm_cube_vs.model, scale_cube);
        pm_cube_vs.model = glm::rotate_slow((glm::mat4)pm_cube_vs.model, rotate_cube.y, glm::vec3(0, 1, 0));
        pm_cube_vs.model = glm::rotate_slow((glm::mat4)pm_cube_vs.model, rotate_cube.x, glm::vec3(1, 0, 0));
        pm_cube_vs.model = glm::rotate_slow((glm::mat4)pm_cube_vs.model, rotate_cube.z, glm::vec3(0, 0, 1));

        cube_pipe.bind();
        cube_pipe.set_params(pm_cube_vs);
        cube_pipe.set_params(pm_cube_fs);
        mesh.draw();

        pm_cube_vs.model = glm::translate(glm::mat4(1), lights.lg[0].light_pos);
        pm_cube_vs.model = glm::scale((glm::mat4)pm_cube_vs.model, scale_light);

        light_pipe.bind();

        light_pipe.set_params(pm_cube_vs);
        light_pipe.set_params(pm_light_fs);
        mesh.draw();

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