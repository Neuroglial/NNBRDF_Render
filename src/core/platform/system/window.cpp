#include "core/platform/system/window.hpp"
#include <stdexcept>


void framebuffer_size_callback(GLFWwindow* window, int width, int height){
}


void Windows::init(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}


void Windows::creat_window(const std::string& name,int width, int height)
{
    window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
    if (window == NULL)
    {
        throw std::runtime_error("Failed to create GLFW window");
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);

}

GLFWwindow* Windows::get_window(){
    return window;
}


void Windows::swapBuffer()
{
    glfwSwapBuffers(window);
    glfwPollEvents();
}