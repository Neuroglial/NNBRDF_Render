#include "core/platform/renderAPI/ShaderParam.hpp"

void print_params(Shader::ParamList &plb)
{
    for (auto &param : plb.m_param_list)
    {
        switch (param.second.m_type)
        {
        case Shader::Param_Type::Float:
            std::cout << "float " + param.first + " = " + std::to_string(*(float *)param.second.m_value_ptr) << std::endl;
            break;
        case Shader::Param_Type::Vec2:
            std::cout << "vec2 " + param.first + " = " + std::to_string(*(glm::vec2 *)param.second.m_value_ptr) << std::endl;
            break;
        case Shader::Param_Type::Vec3:
            std::cout << "vec3 " + param.first + " = " + std::to_string(*(glm::vec3 *)param.second.m_value_ptr) << std::endl;
            break;
        case Shader::Param_Type::Vec4:
            std::cout << "vec4 " + param.first + " = " + std::to_string(*(glm::vec4 *)param.second.m_value_ptr) << std::endl;
            break;

        default:
            break;
        }
    }
}

#define PARAM_HELPER_TYPE_TO_STRING(obj, type) \
    else if (obj == type)                      \
    {                                          \
        return #type;                          \
    }

#define PARAM_HELPER_ALLOC(obj, enum_name, type) \
    else if (obj == enum_name)                   \
    {                                            \
        return new type();                       \
    }

#define PARAM_HELPER_DEL(obj, enum_name, type, ptr) \
    else if (obj == enum_name)                      \
    {                                               \
        delete static_cast<type *>(ptr);            \
    }

#define PARAM_HELPER_SET(obj, enum_name, type, drc, src)       \
    else if (obj == enum_name)                                 \
    {                                                          \
        *static_cast<type *>(drc) = *static_cast<type *>(src); \
    }

std::string Shader::Param_Helper::to_string(Param_Type type)
{
    if (type == None)
    {
        return "None";
    }
    PARAM_HELPER_TYPE_TO_STRING(type, Float)
    PARAM_HELPER_TYPE_TO_STRING(type, Vec2)
    PARAM_HELPER_TYPE_TO_STRING(type, Vec3)
    PARAM_HELPER_TYPE_TO_STRING(type, Vec4)

    return "None";
};

void *Shader::Param_Helper::alloc(Param_Type type)
{
    if (type == None)
    {
        return nullptr;
    }
    PARAM_HELPER_ALLOC(type, Float, float)
    PARAM_HELPER_ALLOC(type, Vec2, glm::vec2)
    PARAM_HELPER_ALLOC(type, Vec3, glm::vec3)
    PARAM_HELPER_ALLOC(type, Vec4, glm::vec4)

    return nullptr;
}

void Shader::Param_Helper::del(void *ptr, Shader::Param_Type type)
{
    if (type == None)
    {
        return;
    }
    PARAM_HELPER_DEL(type, Float, float, ptr)
    PARAM_HELPER_DEL(type, Vec2, glm::vec2, ptr)
    PARAM_HELPER_DEL(type, Vec3, glm::vec3, ptr)
    PARAM_HELPER_DEL(type, Vec4, glm::vec4, ptr)
}

void Shader::Param_Helper::set(void *ptr_d, void * const ptr_s, Shader::Param_Type type)
{
    if (type == None)
    {
        return;
    }
    PARAM_HELPER_SET(type,Float,float,ptr_d,ptr_s)
    PARAM_HELPER_SET(type,Vec2,glm::vec2,ptr_d,ptr_s)
    PARAM_HELPER_SET(type,Vec3,glm::vec3,ptr_d,ptr_s)
    PARAM_HELPER_SET(type,Vec4,glm::vec4,ptr_d,ptr_s)

}
