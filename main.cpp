#include <iostream>
#include "core/platform/system/window.hpp"


int main()
{
    try{
        Windows window;
        window.init();
        window.creat_window("NNBRDF-Render",960,680);
        
        while(!window.shouldClose()){
            window.swapBuffer();
        }


    }catch(std::runtime_error& e){
        std::cout<<"Runtime_Error: "<<e.what()<<std::endl;
    }
}