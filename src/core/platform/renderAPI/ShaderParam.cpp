// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#include "core/platform/renderAPI/ShaderParam.hpp"

void print_params(ShaderParamList &plb)
{
    for (auto &param : plb.m_param_list)
    {
        switch (param.second.m_type)
        {
        case ShaderParam_Type::Int:
            std::cout << "int " + param.first + " = " + std::to_string(*(int *)param.second.m_value_ptr) << std::endl;
            break;
        case ShaderParam_Type::Float:
            std::cout << "float " + param.first + " = " + std::to_string(*(float *)param.second.m_value_ptr) << std::endl;
            break;
        case ShaderParam_Type::Vec2:
            std::cout << "vec2 " + param.first + " = " + std::to_string(*(glm::vec2 *)param.second.m_value_ptr) << std::endl;
            break;
        case ShaderParam_Type::Vec3:
            std::cout << "vec3 " + param.first + " = " + std::to_string(*(glm::vec3 *)param.second.m_value_ptr) << std::endl;
            break;
        case ShaderParam_Type::Vec4:
            std::cout << "vec4 " + param.first + " = " + std::to_string(*(glm::vec4 *)param.second.m_value_ptr) << std::endl;
            break;
        case ShaderParam_Type::Mat2:
            std::cout << "vec4 " + param.first + " = " + std::to_string(*(glm::mat2 *)param.second.m_value_ptr) << std::endl;
            break;
        case ShaderParam_Type::Mat3:
            std::cout << "vec4 " + param.first + " = " + std::to_string(*(glm::mat3 *)param.second.m_value_ptr) << std::endl;
            break;
        case ShaderParam_Type::Mat4:
            std::cout << "vec4 " + param.first + " = " + std::to_string(*(glm::mat4 *)param.second.m_value_ptr) << std::endl;
            break;

        case ShaderParam_Type::Texture2D:
            std::cout << "Texture2D " + param.first + " = " + (*(Ref<Texture2D> *)param.second.m_value_ptr)->get_path() << std::endl;
            break;

        case ShaderParam_Type::TextureCube:
            std::cout << "TextureCube " + param.first << std::endl;
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

std::string ShaderParam_Helper::to_string(ShaderParam_Type type)
{
    if (type == ShaderParam_Type::None)
    {
        return "None";
    }
    PARAM_HELPER_TYPE_TO_STRING(type, ShaderParam_Type::Int)

    PARAM_HELPER_TYPE_TO_STRING(type, ShaderParam_Type::Float)
    PARAM_HELPER_TYPE_TO_STRING(type, ShaderParam_Type::Vec2)
    PARAM_HELPER_TYPE_TO_STRING(type, ShaderParam_Type::Vec3)
    PARAM_HELPER_TYPE_TO_STRING(type, ShaderParam_Type::Vec4)

    PARAM_HELPER_TYPE_TO_STRING(type, ShaderParam_Type::Mat2)
    PARAM_HELPER_TYPE_TO_STRING(type, ShaderParam_Type::Mat3)
    PARAM_HELPER_TYPE_TO_STRING(type, ShaderParam_Type::Mat4)

    PARAM_HELPER_TYPE_TO_STRING(type, ShaderParam_Type::Texture2D)
    PARAM_HELPER_TYPE_TO_STRING(type, ShaderParam_Type::TextureCube)

    return "None";
};

void *ShaderParam_Helper::alloc(ShaderParam_Type type)
{
    if (type == ShaderParam_Type::None)
    {
        return nullptr;
    }
    PARAM_HELPER_ALLOC(type, ShaderParam_Type::Int, int)

    PARAM_HELPER_ALLOC(type, ShaderParam_Type::Float, float)
    PARAM_HELPER_ALLOC(type, ShaderParam_Type::Vec2, glm::vec2)
    PARAM_HELPER_ALLOC(type, ShaderParam_Type::Vec3, glm::vec3)
    PARAM_HELPER_ALLOC(type, ShaderParam_Type::Vec4, glm::vec4)

    PARAM_HELPER_ALLOC(type, ShaderParam_Type::Mat2, glm::mat2)
    PARAM_HELPER_ALLOC(type, ShaderParam_Type::Mat3, glm::mat3)
    PARAM_HELPER_ALLOC(type, ShaderParam_Type::Mat4, glm::mat4)

    PARAM_HELPER_ALLOC(type, ShaderParam_Type::Texture2D, Ref<Texture2D>)
    PARAM_HELPER_ALLOC(type, ShaderParam_Type::TextureCube, Ref<TextureCube>)

    return nullptr;
}

void ShaderParam_Helper::del(void *ptr, ShaderParam_Type type)
{
    if (type == ShaderParam_Type::None)
    {
        return;
    }
    PARAM_HELPER_DEL(type, ShaderParam_Type::Int, int, ptr)

    PARAM_HELPER_DEL(type, ShaderParam_Type::Float, float, ptr)
    PARAM_HELPER_DEL(type, ShaderParam_Type::Vec2, glm::vec2, ptr)
    PARAM_HELPER_DEL(type, ShaderParam_Type::Vec3, glm::vec3, ptr)
    PARAM_HELPER_DEL(type, ShaderParam_Type::Vec4, glm::vec4, ptr)

    PARAM_HELPER_DEL(type, ShaderParam_Type::Mat2, glm::mat2, ptr)
    PARAM_HELPER_DEL(type, ShaderParam_Type::Mat3, glm::mat3, ptr)
    PARAM_HELPER_DEL(type, ShaderParam_Type::Mat4, glm::mat4, ptr)

    PARAM_HELPER_DEL(type, ShaderParam_Type::Texture2D, Ref<Texture2D>, ptr)
    PARAM_HELPER_DEL(type, ShaderParam_Type::TextureCube, Ref<TextureCube>, ptr)
}

void ShaderParam_Helper::set(void *ptr_d, void *const ptr_s, ShaderParam_Type type)
{
    if (type == ShaderParam_Type::None)
    {
        return;
    }
    PARAM_HELPER_SET(type, ShaderParam_Type::Int, int, ptr_d, ptr_s)

    PARAM_HELPER_SET(type, ShaderParam_Type::Float, float, ptr_d, ptr_s)
    PARAM_HELPER_SET(type, ShaderParam_Type::Vec2, glm::vec2, ptr_d, ptr_s)
    PARAM_HELPER_SET(type, ShaderParam_Type::Vec3, glm::vec3, ptr_d, ptr_s)
    PARAM_HELPER_SET(type, ShaderParam_Type::Vec4, glm::vec4, ptr_d, ptr_s)

    PARAM_HELPER_SET(type, ShaderParam_Type::Mat2, glm::mat2, ptr_d, ptr_s)
    PARAM_HELPER_SET(type, ShaderParam_Type::Mat3, glm::mat3, ptr_d, ptr_s)
    PARAM_HELPER_SET(type, ShaderParam_Type::Mat4, glm::mat4, ptr_d, ptr_s)

    PARAM_HELPER_SET(type, ShaderParam_Type::Texture2D, Ref<Texture2D>, ptr_d, ptr_s)
    PARAM_HELPER_SET(type, ShaderParam_Type::TextureCube, Ref<TextureCube>, ptr_d, ptr_s)
}
