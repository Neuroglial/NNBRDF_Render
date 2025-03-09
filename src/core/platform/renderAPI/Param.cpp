// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#include "core/platform/renderAPI/Param.hpp"

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

std::string ShaderParam_Helper::to_string(Param_Type type)
{
    if (type == Param_Type::None)
    {
        return "None";
    }
    PARAM_HELPER_TYPE_TO_STRING(type, Param_Type::Int)

    PARAM_HELPER_TYPE_TO_STRING(type, Param_Type::Float)
    PARAM_HELPER_TYPE_TO_STRING(type, Param_Type::Vec2)
    PARAM_HELPER_TYPE_TO_STRING(type, Param_Type::Vec3)
    PARAM_HELPER_TYPE_TO_STRING(type, Param_Type::Vec4)

    PARAM_HELPER_TYPE_TO_STRING(type, Param_Type::Mat2)
    PARAM_HELPER_TYPE_TO_STRING(type, Param_Type::Mat3)
    PARAM_HELPER_TYPE_TO_STRING(type, Param_Type::Mat4)

    PARAM_HELPER_TYPE_TO_STRING(type, Param_Type::Texture2D)
    PARAM_HELPER_TYPE_TO_STRING(type, Param_Type::TextureCube)

    return "None";
};

void *ShaderParam_Helper::alloc(Param_Type type)
{
    if (type == Param_Type::None)
    {
        return nullptr;
    }
    PARAM_HELPER_ALLOC(type, Param_Type::Int, int)

    PARAM_HELPER_ALLOC(type, Param_Type::Float, float)
    PARAM_HELPER_ALLOC(type, Param_Type::Vec2, glm::vec2)
    PARAM_HELPER_ALLOC(type, Param_Type::Vec3, glm::vec3)
    PARAM_HELPER_ALLOC(type, Param_Type::Vec4, glm::vec4)

    PARAM_HELPER_ALLOC(type, Param_Type::Mat2, glm::mat2)
    PARAM_HELPER_ALLOC(type, Param_Type::Mat3, glm::mat3)
    PARAM_HELPER_ALLOC(type, Param_Type::Mat4, glm::mat4)

    PARAM_HELPER_ALLOC(type, Param_Type::Texture2D, Ref<Texture2D>)
    PARAM_HELPER_ALLOC(type, Param_Type::TextureCube, Ref<TextureCube>)

    return nullptr;
}

void ShaderParam_Helper::del(void *ptr, Param_Type type)
{
    if (type == Param_Type::None)
    {
        return;
    }
    PARAM_HELPER_DEL(type, Param_Type::Int, int, ptr)

    PARAM_HELPER_DEL(type, Param_Type::Float, float, ptr)
    PARAM_HELPER_DEL(type, Param_Type::Vec2, glm::vec2, ptr)
    PARAM_HELPER_DEL(type, Param_Type::Vec3, glm::vec3, ptr)
    PARAM_HELPER_DEL(type, Param_Type::Vec4, glm::vec4, ptr)

    PARAM_HELPER_DEL(type, Param_Type::Mat2, glm::mat2, ptr)
    PARAM_HELPER_DEL(type, Param_Type::Mat3, glm::mat3, ptr)
    PARAM_HELPER_DEL(type, Param_Type::Mat4, glm::mat4, ptr)

    PARAM_HELPER_DEL(type, Param_Type::Texture2D, Ref<Texture2D>, ptr)
    PARAM_HELPER_DEL(type, Param_Type::TextureCube, Ref<TextureCube>, ptr)
}

void ShaderParam_Helper::set(void *ptr_d, void *const ptr_s, Param_Type type)
{
    if (type == Param_Type::None)
    {
        return;
    }
    PARAM_HELPER_SET(type, Param_Type::Int, int, ptr_d, ptr_s)

    PARAM_HELPER_SET(type, Param_Type::Float, float, ptr_d, ptr_s)
    PARAM_HELPER_SET(type, Param_Type::Vec2, glm::vec2, ptr_d, ptr_s)
    PARAM_HELPER_SET(type, Param_Type::Vec3, glm::vec3, ptr_d, ptr_s)
    PARAM_HELPER_SET(type, Param_Type::Vec4, glm::vec4, ptr_d, ptr_s)

    PARAM_HELPER_SET(type, Param_Type::Mat2, glm::mat2, ptr_d, ptr_s)
    PARAM_HELPER_SET(type, Param_Type::Mat3, glm::mat3, ptr_d, ptr_s)
    PARAM_HELPER_SET(type, Param_Type::Mat4, glm::mat4, ptr_d, ptr_s)

    PARAM_HELPER_SET(type, Param_Type::Texture2D, Ref<Texture2D>, ptr_d, ptr_s)
    PARAM_HELPER_SET(type, Param_Type::TextureCube, Ref<TextureCube>, ptr_d, ptr_s)
}
