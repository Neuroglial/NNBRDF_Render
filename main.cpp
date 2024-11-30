#include <iostream>
#include "core/platform/system/window.hpp"
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