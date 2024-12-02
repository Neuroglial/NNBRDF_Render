/*
#include <iostream>
#include "core/platform/system/Window.hpp"
#include "core/platform/system/EventManager.hpp"

void printEvent(Event::Base& event){
    std::cout<<event.get_type_str()<<std::endl;
}

int main()
{
    try{
        Windows window;
        window.init();
        window.creat_window("NNBRDF-Render",960,680);
        EventManage::instance.registerCallback(printEvent);


        while(!window.shouldClose()){
            window.swapBuffer();
        }


    }catch(std::runtime_error& e){
        std::cout<<"Runtime_Error: "<<e.what()<<std::endl;

        getchar();
    }
}
*/

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

    ImageManager::register_image("../source/image/container.jpg");
    ImageManager::register_image("../source/image/awesomeface.png");

    // texture 1
    // ---------
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1); 
     // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    Ref<Image> tex1 = ImageManager::get("../source/image/container.jpg");
    if (tex1)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, tex1->m_width, tex1->m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, tex1->m_data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    // texture 2
    // ---------
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    // set the texture wrapping parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	// set texture wrapping to GL_REPEAT (default wrapping method)
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    // set texture filtering parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // load image, create texture and generate mipmaps
    Ref<Image> tex2 = ImageManager::get("../source/image/awesomeface.png");
    if (tex2)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, tex2->m_width, tex2->m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, tex2->m_data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    VertexShader::Parameters vs_params;
    FragmentShader::Parameters fs_params;

    fs_params.texture1 = 0;
    fs_params.texture2 = 1;


    while (!window.shouldClose())
    {
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        fs_params.color1.get().x = sin(glfwGetTime()*.3)*.5;
        fs_params.color1.get().y = sin(glfwGetTime()*.7)*.5;
        fs_params.color1.get().z = sin(glfwGetTime()*.11)*.5;


        pipe.set_params(vs_params);
        pipe.set_params(fs_params);

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture1);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, texture2);

        pipe.bind();
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        window.swapBuffer();
    }

    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    return 0;
}

void frame_resize(Event::Event& _event){
    EVENT_IF(Event::Event_Frame_Resize,event,_event);
    glViewport(0,0,event.m_width,event.m_height);
    std::cout<<event.get_event()<<std::endl;
}