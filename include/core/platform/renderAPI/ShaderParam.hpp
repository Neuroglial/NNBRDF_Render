#pragma once
#include <iostream>
#include <map>
#include <string>
#include <glm/glm.hpp>
#include <memory>
#include "utils/utils.hpp"

namespace Shader
{
    enum Parameter_Type{
        Float,
        Vec2,
        Vec3,
        Vec4,
    };

    struct Parameter{
        Parameter_Type m_type;
        void* m_value_ptr;

        Parameter(Parameter_Type type,void* ptr):m_type(type),m_value_ptr(ptr){}
    };

    struct ParamListBase{
        std::map<std::string,Parameter> m_param_list;

        std::shared_ptr<ParamListBase> create(){
        }

        ~ParamListBase(){
            if(alloc){
                for(auto& i : m_param_list)
                    free(i.second.m_value_ptr);
            }
        }
        private:
        bool alloc = false;
    };

    #define SHADER_TYPE_REG(BaseTypeName,EumnTypeName,RegTypeName)                      \
    struct RegTypeName{                                                                 \
	    RegTypeName(ParamListBase* pl,const std::string& name){                         \
            pl->m_param_list.insert(std::pair(name,Parameter(EumnTypeName,&m_value)));  \
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


    #define BEGIN_SHADER_PARAM_STRUCT() struct Parameters : ParamListBase {
    #define END_SHADER_PARAM_STRUCT() };
    
    SHADER_TYPE_REG(float,Parameter_Type::Float,SDFloat)
    SHADER_TYPE_REG(glm::vec2,Parameter_Type::Vec2,SDVec2)
    SHADER_TYPE_REG(glm::vec3,Parameter_Type::Vec3,SDVec3)
    SHADER_TYPE_REG(glm::vec4,Parameter_Type::Vec4,SDVec4)

    #define SHADER_PARAM(TypeName,Name) TypeName Name = TypeName(this,#Name);

}

void print_params(Shader::ParamListBase &plb);