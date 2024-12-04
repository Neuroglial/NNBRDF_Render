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

void frame_resize(Event::Event &_event);
void keyborad_input(Event::Event &_event);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    EventManager event_mgr;
    event_mgr.registerCallback(frame_resize);
    event_mgr.registerCallback(keyborad_input);

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

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    std::vector<float> vertex_data = {
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 1.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, -0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f, 0.0f, 1.0f,

        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, -0.5f, 1.0f, 1.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        0.5f, 0.5f, 0.5f, 1.0f, 0.0f,
        -0.5f, 0.5f, 0.5f, 0.0f, 0.0f,
        -0.5f, 0.5f, -0.5f, 0.0f, 1.0f};

    std::vector<unsigned int> index_data = {
        0, 1, 2, 3, 4, 5,
        6, 7, 8, 9, 10, 11,
        12, 13, 14, 15, 16, 17,
        18, 19, 20, 21, 22, 23,
        24, 25, 26, 27, 28, 29,
        30, 31, 32, 33, 34, 35};

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

    Ref<ArrayBuffer_GL<unsigned int>> EBO = std::make_shared<ArrayBuffer_GL<unsigned int>>();
    Ref<ArrayBuffer_GL<float>> VBO = std::make_shared<ArrayBuffer_GL<float>>();
    EBO->set_data(index_data);
    VBO->set_data(vertex_data);

    Mesh_GL mesh;

    mesh.set_buffer(VBO, EBO, {Shader::Param_Type::Vec3, Shader::Param_Type::Vec2});

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

    while (!window.shouldClose())
    {

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        view = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -3.0f));

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

void frame_resize(Event::Event &_event)
{
    EVENT_IF(Event::Event_Frame_Resize, event, _event);
    glViewport(0, 0, event.m_width, event.m_height);
    std::cout << event.get_event() << std::endl;
}

void keyborad_input(Event::Event &_event)
{
    EVENT_IF(Event::Event_Keyboard, event, _event);

    std::cout << event.get_event() << std::endl;
}