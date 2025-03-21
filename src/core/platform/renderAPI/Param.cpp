// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#include "core/platform/renderAPI/Param.hpp"

#define PARAM_HELPER_TYPE_TO_STRING(obj, type) \
    else if (obj == ParamType::type)           \
    {                                          \
        return #type;                          \
    }

#define PARAM_HELPER_STRING_TO_TYPE(str, type) \
    else if (str == #type)                     \
    {                                          \
        return ParamType::type;                \
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

std::string ParamHelper::to_string(ParamType type)
{
    if (type == ParamType::None)
    {
        return "None";
    }
    PARAM_HELPER_TYPE_TO_STRING(type, Int)

    PARAM_HELPER_TYPE_TO_STRING(type, Float)
    PARAM_HELPER_TYPE_TO_STRING(type, Vec2)
    PARAM_HELPER_TYPE_TO_STRING(type, Vec3)
    PARAM_HELPER_TYPE_TO_STRING(type, Vec4)

    PARAM_HELPER_TYPE_TO_STRING(type, Mat2)
    PARAM_HELPER_TYPE_TO_STRING(type, Mat3)
    PARAM_HELPER_TYPE_TO_STRING(type, Mat4)

    PARAM_HELPER_TYPE_TO_STRING(type, Texture2D)
    PARAM_HELPER_TYPE_TO_STRING(type, TextureCube)

    return "None";
};

ParamType ParamHelper::to_type(const std::string &str)
{
    if (str == "None")
    {
        return ParamType::None;
    }
    PARAM_HELPER_STRING_TO_TYPE(str, Int)

    PARAM_HELPER_STRING_TO_TYPE(str, Float)
    PARAM_HELPER_STRING_TO_TYPE(str, Vec2)
    PARAM_HELPER_STRING_TO_TYPE(str, Vec3)
    PARAM_HELPER_STRING_TO_TYPE(str, Vec4)

    PARAM_HELPER_STRING_TO_TYPE(str, Mat2)
    PARAM_HELPER_STRING_TO_TYPE(str, Mat3)
    PARAM_HELPER_STRING_TO_TYPE(str, Mat4)

    PARAM_HELPER_STRING_TO_TYPE(str, Texture2D)
    PARAM_HELPER_STRING_TO_TYPE(str, TextureCube)

    return ParamType::None;
}

void *ParamHelper::alloc(ParamType type)
{
    if (type == ParamType::None)
    {
        return nullptr;
    }
    PARAM_HELPER_ALLOC(type, ParamType::Int, int)

    PARAM_HELPER_ALLOC(type, ParamType::Float, float)
    PARAM_HELPER_ALLOC(type, ParamType::Vec2, glm::vec2)
    PARAM_HELPER_ALLOC(type, ParamType::Vec3, glm::vec3)
    PARAM_HELPER_ALLOC(type, ParamType::Vec4, glm::vec4)

    PARAM_HELPER_ALLOC(type, ParamType::Mat2, glm::mat2)
    PARAM_HELPER_ALLOC(type, ParamType::Mat3, glm::mat3)
    PARAM_HELPER_ALLOC(type, ParamType::Mat4, glm::mat4)

    PARAM_HELPER_ALLOC(type, ParamType::Texture2D, Ref<Texture2D>)
    PARAM_HELPER_ALLOC(type, ParamType::TextureCube, Ref<TextureCube>)

    return nullptr;
}

void ParamHelper::del(void *ptr, ParamType type)
{
    if (type == ParamType::None)
    {
        return;
    }
    PARAM_HELPER_DEL(type, ParamType::Int, int, ptr)

    PARAM_HELPER_DEL(type, ParamType::Float, float, ptr)
    PARAM_HELPER_DEL(type, ParamType::Vec2, glm::vec2, ptr)
    PARAM_HELPER_DEL(type, ParamType::Vec3, glm::vec3, ptr)
    PARAM_HELPER_DEL(type, ParamType::Vec4, glm::vec4, ptr)

    PARAM_HELPER_DEL(type, ParamType::Mat2, glm::mat2, ptr)
    PARAM_HELPER_DEL(type, ParamType::Mat3, glm::mat3, ptr)
    PARAM_HELPER_DEL(type, ParamType::Mat4, glm::mat4, ptr)

    PARAM_HELPER_DEL(type, ParamType::Texture2D, Ref<Texture2D>, ptr)
    PARAM_HELPER_DEL(type, ParamType::TextureCube, Ref<TextureCube>, ptr)
}

void ParamHelper::set(void *ptr_d, void *const ptr_s, ParamType type)
{
    if (type == ParamType::None)
    {
        return;
    }
    PARAM_HELPER_SET(type, ParamType::Int, int, ptr_d, ptr_s)

    PARAM_HELPER_SET(type, ParamType::Float, float, ptr_d, ptr_s)
    PARAM_HELPER_SET(type, ParamType::Vec2, glm::vec2, ptr_d, ptr_s)
    PARAM_HELPER_SET(type, ParamType::Vec3, glm::vec3, ptr_d, ptr_s)
    PARAM_HELPER_SET(type, ParamType::Vec4, glm::vec4, ptr_d, ptr_s)

    PARAM_HELPER_SET(type, ParamType::Mat2, glm::mat2, ptr_d, ptr_s)
    PARAM_HELPER_SET(type, ParamType::Mat3, glm::mat3, ptr_d, ptr_s)
    PARAM_HELPER_SET(type, ParamType::Mat4, glm::mat4, ptr_d, ptr_s)

    PARAM_HELPER_SET(type, ParamType::Texture2D, Ref<Texture2D>, ptr_d, ptr_s)
    PARAM_HELPER_SET(type, ParamType::TextureCube, Ref<TextureCube>, ptr_d, ptr_s)
}

void Params::add(Ref<Param> param)
{
    ref_Params.push_back(param);
    m_list.emplace(param->name(), param.get());
}

void Params::add(Param *param)
{
    m_list.emplace(param->name(), param);
}

void Params::get(const Params &other)
{
    for (auto &i : m_list)
        if (auto fd = other[i.first])
            i.second->set(*fd);
}

Ref<Params> Params::copy()
{

    Ref<Params> cpy(new Params);

    for (auto i : m_list)
        cpy->add(Ref<Param>(i.second->copy()));
    return cpy;
}
