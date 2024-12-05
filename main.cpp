#include <glad/glad.h>
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

    MyCamera camera = MyCamera(45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, ProjectMode::Persp);
    event_mgr.registerCallback(std::bind(Object::callback, &camera, std::placeholders::_1));

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    glEnable(GL_DEPTH_TEST);

    // world space positions of our cubes
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f, 0.0f, 0.0f),
        glm::vec3(2.0f, 5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f, 3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f, 2.0f, -2.5f),
        glm::vec3(1.5f, 0.2f, -1.5f),
        glm::vec3(-1.3f, 1.0f, -1.5f)};

    Shader::Pipline_GL cube_pipe;
    Shader::Pipline_GL light_pipe;

    Mesh_GL mesh(Mesh::Cube);

    cube_pipe.attach_shader(ShaderManager::get("../source/shaders/cube.vs"));
    cube_pipe.attach_shader(ShaderManager::get("../source/shaders/cube.fs"));

    light_pipe.attach_shader(ShaderManager::get("../source/shaders/cube.vs"));
    light_pipe.attach_shader(ShaderManager::get("../source/shaders/light.fs"));

    auto vs_pms = ShaderManager::get("../source/shaders/cube.vs")->get_params();
    auto cube_fs_pms = ShaderManager::get("../source/shaders/cube.fs")->get_params();
    auto light_fs_pms = ShaderManager::get("../source/shaders/light.fs")->get_params();

    auto &projection = PTR_AS(glm::mat4, vs_pms["projection"].m_value_ptr);
    auto &view = PTR_AS(glm::mat4, vs_pms["view"].m_value_ptr);
    auto &model = PTR_AS(glm::mat4, vs_pms["model"].m_value_ptr);

    auto &texture1 = PTR_AS(Ref<Texture::Texture2D>, cube_fs_pms["texture1"].m_value_ptr);
    auto &texture2 = PTR_AS(Ref<Texture::Texture2D>, cube_fs_pms["texture2"].m_value_ptr);

    auto &lightPos = PTR_AS(glm::vec3, cube_fs_pms["lightPos"].m_value_ptr);
    auto &viewPos = PTR_AS(glm::vec3, cube_fs_pms["viewPos"].m_value_ptr);
    auto &lightColor = PTR_AS(glm::vec3, cube_fs_pms["lightColor"].m_value_ptr);

    auto &lightColor2 = PTR_AS(glm::vec3, light_fs_pms["lightColor"].m_value_ptr);

    texture1 = std::make_shared<Texture::Texture2D_GL>(
        Texture::Warpping_Mode::CLAMP, Texture::Filtering_Mode::Mipmap);
    texture1->set_image(ImageManager::get("../source/image/container.jpg"));

    texture2 = std::make_shared<Texture::Texture2D_GL>(
        Texture::Warpping_Mode::CLAMP, Texture::Filtering_Mode::Mipmap);
    texture2->set_image(ImageManager::get("../source/image/awesomeface.png"));

    glm::vec3 l_pos(0.0f, 0.0f, -3.2f);
    glm::vec3 l_color(1.0f,0.95f,0.91f);

    glm::vec4 l_tans(6.0f,0.0f,0.0f,1.0f);
    float l_rot = 0.0f;

    lightColor2 = l_color;
    lightColor = l_color;

    double time = glfwGetTime();
    double last = time;
    double delta = 0.0;

    while (!window.shouldClose())
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        time = glfwGetTime();
        delta = time - last;
        last = time;

        camera.tick(delta);
        l_rot += 5.0f;
        l_pos = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -4.0f)) * glm::rotate(glm::mat4(1.0f), glm::radians(l_rot)*0.02f, glm::vec3(0.11f, 1.0f, 0.3f)) * l_tans;

        projection = camera.m_camera.get_view();
        view = glm::inverse(camera.get_model());

        viewPos = camera.get_position();
        lightPos = l_pos;

        cube_pipe.bind();
        cube_pipe.set_params(vs_pms);
        cube_pipe.set_params(cube_fs_pms);

        for (unsigned int i = 0; i < 10; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

            cube_pipe.set_params("model", vs_pms["model"]);
            mesh.draw();
        }

        model = glm::mat4(1.0f);
        model = glm::translate(model, l_pos);
        model = glm::scale(model, glm::vec3(0.15f));

        light_pipe.bind();
        light_pipe.set_params(vs_pms);
        light_pipe.set_params(light_fs_pms);

        mesh.draw();

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