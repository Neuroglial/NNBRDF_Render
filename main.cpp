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

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader::Pipline_GL pipe;

    pipe.attach_shader(ShaderManager::get("../vertex_shader.glsl"));
    pipe.attach_shader(ShaderManager::get("../fragment_shader.glsl"));
    auto vs_params = ShaderManager::get("../vertex_shader.glsl")->get_params();
    auto fs_params = ShaderManager::get("../fragment_shader.glsl")->get_params();
    pipe.bind();

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


    Mesh_GL mesh(Mesh::Cube`);

    auto &texture1 = PTR_AS(Ref<Texture::Texture2D>, fs_params["texture1"].m_value_ptr);
    auto &texture2 = PTR_AS(Ref<Texture::Texture2D>, fs_params["texture2"].m_value_ptr);

    texture1 = std::make_shared<Texture::Texture2D_GL>(
        Texture::Warpping_Mode::CLAMP, Texture::Filtering_Mode::Mipmap);

    texture2 = std::make_shared<Texture::Texture2D_GL>(
        Texture::Warpping_Mode::CLAMP, Texture::Filtering_Mode::Mipmap);

    texture1->set_image(ImageManager::get("../source/image/container.jpg"));
    texture2->set_image(ImageManager::get("../source/image/awesomeface.png"));

    auto &projection = PTR_AS(glm::mat4, vs_params["projection"].m_value_ptr);
    auto &view = PTR_AS(glm::mat4, vs_params["view"].m_value_ptr);
    auto &model = PTR_AS(glm::mat4, vs_params["model"].m_value_ptr);

    glEnable(GL_DEPTH_TEST);

    MyCamera camera = MyCamera(45.0f, (float)SCR_WIDTH / (float)SCR_HEIGHT, ProjectMode::Persp);
    event_mgr.registerCallback(std::bind(Object::callback, &camera, std::placeholders::_1));

    double time = glfwGetTime();
    double last = time;
    double delta = 0.0;

    while (!window.shouldClose())
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        time = glfwGetTime();
        delta = last - time;
        last = time;

        camera.tick(delta);

        projection = camera.m_camera.get_view();
        view = glm::inverse(camera.get_model());

        pipe.bind();
        pipe.set_params(fs_params);
        pipe.set_params(vs_params);

        for (unsigned int i = 0; i < 10; i++)
        {
            model = glm::mat4(1.0f);
            model = glm::translate(model, cubePositions[i]);
            float angle = 20.0f * i;
            model = glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

            pipe.set_params("model", vs_params["model"]);

            pipe.bind();
            mesh.draw();
        }

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