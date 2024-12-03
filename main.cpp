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
#include "core/platform/renderAPI/OpenGL/ArrayBuffer_GL.hpp"
#include "core/platform/renderAPI/OpenGL/Mesh_GL.hpp"

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
    auto vs_params = ShaderManager::get("../vertex_shader.glsl")->get_params();
    auto fs_params = ShaderManager::get("../fragment_shader.glsl")->get_params();
    pipe.bind();

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
    std::vector<float> vertex_data = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };

    std::vector<unsigned int> index_data = { 0, 1, 3, 1, 2, 3 };

    Ref<ArrayBuffer_GL<unsigned int>> EBO = std::make_shared<ArrayBuffer_GL<unsigned int>>();
    Ref<ArrayBuffer_GL<float>> VBO = std::make_shared<ArrayBuffer_GL<float>>();
    EBO->set_data(index_data);
    VBO->set_data(vertex_data);

    Mesh_GL mesh;

    mesh.set_buffer(VBO,EBO,{Shader::Param_Type::Vec3,Shader::Param_Type::Vec3,Shader::Param_Type::Vec2});

    auto& texture1 = PTR_AS(Ref<Texture::Texture2D>,fs_params["texture1"].m_value_ptr);
    auto& texture2 = PTR_AS(Ref<Texture::Texture2D>,fs_params["texture2"].m_value_ptr);
    glm::vec3& color1 = *((glm::vec3*)fs_params["color1"].m_value_ptr);


    texture1 = std::make_shared<Texture::Texture2D_GL>(
       Texture::Warpping_Mode::CLAMP,Texture::Filtering_Mode::Mipmap);
    
    texture2 = std::make_shared<Texture::Texture2D_GL>(
        Texture::Warpping_Mode::CLAMP,Texture::Filtering_Mode::Mipmap);

    texture1->set_image( ImageManager::get("../source/image/container.jpg"));
    texture2->set_image( ImageManager::get("../source/image/awesomeface.png"));


    while (!window.shouldClose())
    {
        
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        color1 = glm::vec3(sin(glfwGetTime())*0.2);

        pipe.bind();
        pipe.set_params(vs_params);
        pipe.set_params(fs_params);

        mesh.bind();
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        window.swapBuffer();
    }



    return 0;
}

void frame_resize(Event::Event& _event){
    EVENT_IF(Event::Event_Frame_Resize,event,_event);
    glViewport(0,0,event.m_width,event.m_height);
    std::cout<<event.get_event()<<std::endl;
}