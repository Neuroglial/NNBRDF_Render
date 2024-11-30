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

#include "core/platform/renderAPI/Shader.hpp"

namespace Shader
{
    SHADER_STRUCT_BEGINE(TestShaderParams)
    SHADER_PARAM_FLOAT(v1)
    SHADER_PARAM_VEC2(v2)
    SHADER_PARAM_VEC3(v3)
    SHADER_PARAM_VEC4(v4)
    SHADER_STRUCT_END
}

int main()
{
    Shader::TestShaderParams testParams;
    
    testParams.v1 = 1;
    testParams.v2 = glm::vec2(2,3);
    testParams.v3 = glm::vec3(4,5,6);
    testParams.v4 = glm::vec4(7,8,9,10);

    print_params(testParams);
    
    return 0;
}
