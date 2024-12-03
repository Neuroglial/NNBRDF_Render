#pragma once
#include <iostream>
#include <map>
#include <string>
#include <glm/glm.hpp>
#include <memory>
#include "utils/utils.hpp"
#include "core/platform/renderAPI/Texture.hpp"

namespace Shader
{
    enum Param_Type{
        None,
        Float,
        Vec2,
        Vec3,
        Vec4,
        Int,
        Texture2D,
    };

    struct Param_Helper{
        static std::string to_string(Param_Type type);
        static void* alloc(Param_Type type);
        static void del(void* ptr,Param_Type type);
        static void set(void* ptr_d,void* const ptr_s,Param_Type type);
    };

    struct Parameter{
        Param_Type m_type;
        void* m_value_ptr;

        Parameter(Param_Type type,void* ptr):m_type(type),m_value_ptr(ptr),m_alloc(false){

        }

        Parameter(Param_Type type):m_type(type),m_alloc(true){
            m_value_ptr = Param_Helper::alloc(m_type);
        }

        Parameter(Parameter&& other) noexcept{
            m_type = other.m_type;
            m_alloc = other.m_alloc;
            m_value_ptr = other.m_value_ptr;

            other.m_alloc = false;
            other.m_value_ptr = nullptr;
            other.m_type = Param_Type::None;
        }

        Parameter(const Parameter& other) noexcept{
            m_type = other.m_type;
            m_alloc = true;
            m_value_ptr = Param_Helper::alloc(m_type);
        }

        ~Parameter(){
            if(m_alloc&&m_value_ptr)
                Param_Helper::del(m_value_ptr,m_type);
        }

        void set(void* value){
            Param_Helper::set(m_value_ptr,value,m_type);
        }

        private:
        bool m_alloc;
    };

    struct ParamList{
        std::map<std::string,Parameter> m_param_list;

        ParamList() = default;

        ParamList(ParamList&& other){
            m_param_list.swap(other.m_param_list);
        }

        ParamList(const ParamList& other){
            *this = other;
        }

        ParamList& operator=(const ParamList& other){
            for(auto& i:other.m_param_list){
                m_param_list.insert(std::pair(i.first,Parameter(i.second.m_type)));
            }

            return *this;
        }

        ParamList& operator=(ParamList&& other){
            m_param_list.swap(other.m_param_list);
            return *this;
        }

        ParamList operator+(const ParamList& other){
            ParamList ret;

            ret = *this;
            for(auto& i:other.m_param_list){
                m_param_list.insert(std::pair(i.first,Parameter(i.second.m_type)));
            }

            return ret;
        }

        Parameter& operator[](const std::string& param_name){
            auto i =m_param_list.find(param_name);
            if(i!=m_param_list.end()){
                return i->second;
            }else{
                throw std::runtime_error("Shader Parameter Named " + param_name + " Don't Exist");
            }
        }
    };

    #define SHADER_TYPE_REG(BaseTypeName,EumnTypeName,RegTypeName)                      \
    struct RegTypeName{                                                                 \
	    RegTypeName(ParamList* pl,const std::string& name){                         \
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
        BaseTypeName& operator()(){                                                     \
            return m_value;                                                             \
        }                                                                               \
                                                                                        \
        BaseTypeName& get(){                                                            \
            return m_value;                                                             \
        }                                                                               \
                                                                                        \
	    private:                                                                        \
	    BaseTypeName m_value;                                                           \
    };                                                                                  \


    #define BEGIN_SHADER_PARAM_STRUCT() struct Parameters : ParamList {
    #define END_SHADER_PARAM_STRUCT() };
    
    SHADER_TYPE_REG(float,Param_Type::Float,SDFloat)
    SHADER_TYPE_REG(glm::vec2,Param_Type::Vec2,SDVec2)
    SHADER_TYPE_REG(glm::vec3,Param_Type::Vec3,SDVec3)
    SHADER_TYPE_REG(glm::vec4,Param_Type::Vec4,SDVec4)
    SHADER_TYPE_REG(Ref<Texture::Texture2D>,Param_Type::Texture2D,SDTexture2D)

    SHADER_TYPE_REG(int,Param_Type::Int,SDInt)

    #define SHADER_PARAM(TypeName,Name) TypeName Name = TypeName(this,#Name);

}

void print_params(Shader::ParamList &plb);