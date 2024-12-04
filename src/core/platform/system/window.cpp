#include "core/platform/system/Window.hpp"
#include "core/platform/system/EventManager.hpp"
#include <stdexcept>

EventManager* manager = nullptr;

void framebuffer_size_callback(GLFWwindow* window, int width, int height){
    Event::Event_Frame_Resize event(width,height);
    manager->triggerEvents(event);
}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods){
    Event::Event_Keyboard event((KeyCode)key,(PressType)action,mods);
    manager->triggerEvents(event);
}

void mouse_button_callback(GLFWwindow* window, int key, int action, int mods){
    Event::Event_Keyboard event((KeyCode)key,(PressType)action,mods);
    manager->triggerEvents(event);
}

void cursor_position_callback(GLFWwindow* window, double xpos, double ypos){
    Event::Event_Mouse_Move event(xpos,ypos);
    manager->triggerEvents(event);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset){
    Event::Event_Scroll event(xoffset,yoffset);
    manager->triggerEvents(event);
}

void Windows::init(){
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
}


void Windows::creat_window(const std::string& name,int width, int height,EventManager& mgr)
{
    window = glfwCreateWindow(width, height, name.c_str(), NULL, NULL);
    if (window == NULL)
    {
        throw std::runtime_error("Failed to create GLFW window");
        glfwTerminate();
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window,framebuffer_size_callback);
    glfwSetKeyCallback(window,key_callback);
    glfwSetMouseButtonCallback(window,mouse_button_callback);
    glfwSetCursorPosCallback(window,cursor_position_callback);
    glfwSetScrollCallback(window,scroll_callback);

    manager = &mgr;
}

GLFWwindow* Windows::get_window(){
    return window;
}


void Windows::swapBuffer()
{
    glfwSwapBuffers(window);
    glfwPollEvents();
}