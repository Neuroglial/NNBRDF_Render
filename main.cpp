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

namespace Shader{

    class VertexShader:public Shader_GL{
        public:

        DECLARE_SHADER(VertexShader)

        BEGIN_SHADER_PARAM_STRUCT(params)
            SHADER_PARAM(SDVec3,color1)
        END_SHADER_PARAM_STRUCT()
    } vs_test1("../3.3.shader.vs",Shader_Type::VERTEX_SHADER);

    class FragmentShader:public Shader_GL{
        public:

        DECLARE_SHADER(FragmentShader)

        BEGIN_SHADER_PARAM_STRUCT(params)
            SHADER_PARAM(SDVec3,color2)
        END_SHADER_PARAM_STRUCT()
    } fs_test1("../3.3.shader.fs",Shader_Type::FRAGMENT_SHADER);


}

void ssframebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

int main()
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, ssframebuffer_size_callback);

    
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    Shader::Pipline_GL pipe;

    pipe.attach_shader(&Shader::vs_test1);
    pipe.attach_shader(&Shader::fs_test1);

    pipe.bind();

    Shader::VertexShader::params pms_vs;
    Shader::FragmentShader::params pms_fs;
    pms_vs.color1 = glm::vec3(0.1,0.1,0.75);
    pms_fs.color2 = glm::vec3(0.1,0.1,0.1);

    pipe.set_params(pms_vs);
    pipe.set_params(pms_fs);

    
    float vertices[] = {
        // positions         // colors
         0.5f, -0.5f, 0.0f,  1.0f, 0.0f, 0.0f,  // bottom right
        -0.5f, -0.5f, 0.0f,  0.0f, 1.0f, 0.0f,  // bottom left
         0.0f,  0.5f, 0.0f,  0.0f, 0.0f, 1.0f   // top 
    };

    unsigned int VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);


    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // render the triangle
        pipe.bind();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void ssframebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}