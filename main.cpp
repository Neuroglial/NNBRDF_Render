#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

#include "core/platform/renderAPI/OpenGL/Shader_GL.hpp"
#include "core/platform/renderAPI/Shader.hpp"
#include "scene/ShaderManager.hpp"
#include "shaders/shaders.hpp"
#include "core/platform/system/Window.hpp"
#include "core/platform/system/EventManager.hpp"
#include "scene/ImageManager.hpp"
#include "core/platform/renderAPI/OpenGL/Texture_GL.hpp"

void frame_resize(Event::Event& _event);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    EventManager event_mgr;
    event_mgr.registerCallback(frame_resize);

    Windows window;
    window.init();
    window.creat_window("NNBRDF_Render",SCR_WIDTH, SCR_HEIGHT,event_mgr);


    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }


    Shader::Pipline_GL pipe;

    pipe.attach_shader(ShaderManager::get("../vertex_shader.glsl"));
    pipe.attach_shader(ShaderManager::get("../fragment_shader.glsl"));
    pipe.bind();

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    float vertices[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texture coord attribute
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);



    // load and create a texture 
    // -------------------------
    unsigned int texture1, texture2;

    VertexShader::Parameters vs_params;
    FragmentShader::Parameters fs_params;

    fs_params.texture1 = std::make_shared<Texture::Texture2D_GL>(
        Texture::Warpping_Mode::CLAMP_TO_EDGE,Texture::Filtering_Mode::LINEAR,true);
    fs_params.texture2 = std::make_shared<Texture::Texture2D_GL>(
        Texture::Warpping_Mode::CLAMP_TO_EDGE,Texture::Filtering_Mode::LINEAR,true);

    fs_params.texture1.get()->set_image( ImageManager::get("../source/image/container.jpg"));
    fs_params.texture2.get()->set_image( ImageManager::get("../source/image/awesomeface.png"));


    while (!window.shouldClose())
    {
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        fs_params.color1.get() = glm::vec3(sin(glfwGetTime()*.3)*.5,
                                           sin(glfwGetTime()*.7)*.5,
                                           sin(glfwGetTime()*.11)*.5);
        pipe.bind();
        pipe.set_params(vs_params);
        pipe.set_params(fs_params);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        window.swapBuffer();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);

    return 0;
}

void frame_resize(Event::Event& _event){
    EVENT_IF(Event::Event_Frame_Resize,event,_event);
    glViewport(0,0,event.m_width,event.m_height);
    std::cout<<event.get_event()<<std::endl;
}