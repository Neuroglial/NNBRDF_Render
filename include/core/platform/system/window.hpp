#pragma once
#include <GLFW/glfw3.h>
#include <vector>
#include <iostream>
#include "core/platform/system/EventManager.hpp"


class Windows{
    public:
    
    void init();
    void creat_window(const std::string& name,int width, int height,EventManager& mgr);
    GLFWwindow* get_window();
    void swapBuffer();


    Windows(){
        window = 0;
    }

    
    inline bool shouldClose() {
        if(window==0)return true;
        return glfwWindowShouldClose(window);}

    private:
    GLFWwindow* window;
};