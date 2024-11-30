#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "utils/utils.hpp"
#include "core/platform/renderAPI/ShaderParam.hpp"

namespace Shader
{
    enum Shader_Type{
        VERTEX_SHADER,
        FRAGMENT_SHADER
    };

    class Shader{
        public:
        Shader(const std::string& path,Shader_Type type):m_path(path),m_type(type){}
        virtual ~Shader(){

        }

        protected:
        std::string m_path;
        Shader_Type m_type;
    };


    class Pipline{
        public:
        void attach_shader(Shader* shader){
            m_shaders.push_back(shader);
        }

        virtual void set_params(ParamListBase& params) = 0;
        virtual void bind() = 0;
        
        virtual ~Pipline(){

        }
        protected:
        std::vector<Shader*> m_shaders;
    };
}
