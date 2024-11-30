#pragma once
#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "utils/utils.hpp"

namespace Shader
{
    enum Type{
        VERTEX_SHADER,
        FRAGMENT_SHADER
    };

    enum Parameter_Type{
        Float,
        Vec2,
        Vec3,
        Vec4,
    };

    struct Parameter{
        Parameter_Type m_type;
        std::string m_name;
        void* m_value_ptr;

        Parameter(Parameter_Type type,const std::string& name,void* ptr):m_type(type),m_name(name),m_value_ptr(ptr){}
    };

    struct ParamListBase{
        std::vector<Parameter> m_param_list;
    };

    #define SHADER_TYPE_REG(BaseTypeName,EumnTypeName,RegTypeName)                      \
    struct RegTypeName{                                                                 \
	    RegTypeName(ParamListBase* pl,const std::string& name){                         \
		    pl->m_param_list.push_back(Parameter(EumnTypeName,name,&m_value));          \
	    }                                                                               \
                                                                                        \
	    operator BaseTypeName() const{                                                  \
		    return m_value;                                                             \
	    }                                                                               \
                                                                                        \
	    RegTypeName& operator=(BaseTypeName value){                                     \
		    m_value = value;                                                            \
		    return *this;                                                               \
	    }                                                                               \
                                                                                        \
	    private:                                                                        \
	    BaseTypeName m_value;                                                           \
    };                                                                                  \


    #define SHADER_STRUCT_BEGINE(StructName) struct StructName : ParamListBase {
    #define SHADER_STRUCT_END };
    
    SHADER_TYPE_REG(float,Parameter_Type::Float,S_Float)
    #define SHADER_PARAM_FLOAT(Name) S_Float Name = S_Float(this,#Name);

    SHADER_TYPE_REG(glm::vec2,Parameter_Type::Vec2,S_Vec2)
    #define SHADER_PARAM_VEC2(Name) S_Vec2 Name = S_Vec2(this,#Name);

    SHADER_TYPE_REG(glm::vec3,Parameter_Type::Vec3,S_Vec3)
    #define SHADER_PARAM_VEC3(Name) S_Vec3 Name = S_Vec3(this,#Name);

    SHADER_TYPE_REG(glm::vec4,Parameter_Type::Vec4,S_Vec4)
    #define SHADER_PARAM_VEC4(Name) S_Vec4 Name = S_Vec4(this,#Name);
}

namespace Shader
{
    class Shader{
        std::vector<Parameter>& get_parameters(){
            return m_parameters;
        }

        private:
        std::vector<Parameter> m_parameters;
    };


    class Pipline{

    };
}

void print_params(Shader::ParamListBase &plb)
{
    for (auto &param : plb.m_param_list)
    {
        switch (param.m_type)
        {
        case Shader::Parameter_Type::Float:
            std::cout << "float " + param.m_name + " = " + std::to_string(*(float *)param.m_value_ptr) << std::endl;
            break;
        case Shader::Parameter_Type::Vec2:
            std::cout << "vec2 " + param.m_name + " = " + std::to_string(*(glm::vec2 *)param.m_value_ptr) << std::endl;
            break;
        case Shader::Parameter_Type::Vec3:
            std::cout << "vec3 " + param.m_name + " = " + std::to_string(*(glm::vec3 *)param.m_value_ptr) << std::endl;
            break;
        case Shader::Parameter_Type::Vec4:
            std::cout << "vec4 " + param.m_name + " = " + std::to_string(*(glm::vec4 *)param.m_value_ptr) << std::endl;
            break;

        default:
            break;
        }
    }
}