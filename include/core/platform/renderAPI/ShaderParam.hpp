// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#pragma once
#include <iostream>
#include <map>
#include <string>
#include <glm/glm.hpp>
#include <memory>
#include "utils/utils.hpp"
#include "core/platform/renderAPI/Texture.hpp"

enum class ShaderParam_Type
{
    None,
    Int,
    Float,
    Vec2,
    Vec3,
    Vec4,
    Mat2,
    Mat3,
    Mat4,
    Texture2D,
    TextureCube,
};

struct ShaderParam_Helper
{
    static std::string to_string(ShaderParam_Type type);
    static void *alloc(ShaderParam_Type type);
    static void del(void *ptr, ShaderParam_Type type);
    static void set(void *ptr_d, void *const ptr_s, ShaderParam_Type type);
};

struct ShaderParam
{
    ShaderParam_Type m_type;
    void *m_value_ptr;

    ShaderParam(ShaderParam_Type type, void *ptr) : m_type(type), m_value_ptr(ptr), m_alloc(false)
    {
    }

    ShaderParam(ShaderParam_Type type) : m_type(type), m_alloc(true)
    {
        m_value_ptr = ShaderParam_Helper::alloc(m_type);
    }

    ShaderParam(ShaderParam &&other) noexcept
    {
        m_type = other.m_type;
        m_alloc = other.m_alloc;
        m_value_ptr = other.m_value_ptr;

        other.m_alloc = false;
        other.m_value_ptr = nullptr;
        other.m_type = ShaderParam_Type::None;
    }

    ShaderParam(const ShaderParam &other) noexcept
    {
        m_type = other.m_type;
        m_alloc = true;
        m_value_ptr = ShaderParam_Helper::alloc(m_type);
    }

    ~ShaderParam()
    {
        if (m_alloc && m_value_ptr)
            ShaderParam_Helper::del(m_value_ptr, m_type);
    }

    void set(void *value)
    {
        ShaderParam_Helper::set(m_value_ptr, value, m_type);
    }

private:
    bool m_alloc;
};

struct ShaderParamList
{
    std::map<std::string, ShaderParam> m_param_list;

    ShaderParamList() = default;

    ShaderParamList(ShaderParamList &&other)
    {
        m_param_list.swap(other.m_param_list);
    }

    ShaderParamList(const ShaderParamList &other)
    {
        *this = other;
    }

    ShaderParamList &operator=(const ShaderParamList &other)
    {
        m_param_list.clear();
        for (auto &i : other.m_param_list)
        {
            m_param_list.insert(std::pair<std::string, ShaderParam>(i.first, ShaderParam(i.second.m_type)));
        }

        return *this;
    }

    ShaderParamList &operator=(ShaderParamList &&other)
    {
        m_param_list.swap(other.m_param_list);
        return *this;
    }

    ShaderParamList operator+(const ShaderParamList &other)
    {
        ShaderParamList ret;

        ret = *this;
        for (auto &i : other.m_param_list)
        {
            ret.m_param_list.insert(std::pair<std::string, ShaderParam>(i.first, ShaderParam(i.second.m_type)));
        }

        return ret;
    }

    ShaderParamList &operator+=(const ShaderParamList &other)
    {
        *this = *this + other;
        return *this;
    }

    ShaderParam &operator[](const std::string &param_name)
    {
        auto i = m_param_list.find(param_name);
        if (i != m_param_list.end())
        {
            return i->second;
        }
        else
        {
            throw std::runtime_error("Shader Parameter Named " + param_name + " Don't Exist");
        }
    }
};

#define SHADER_TYPE_REG(BaseTypeName, EumnTypeName, RegTypeName)                                                     \
    struct RegTypeName                                                                                               \
    {                                                                                                                \
        RegTypeName(ShaderParamList *pl, const std::string &name)                                                    \
        {                                                                                                            \
            pl->m_param_list.insert(std::pair<std::string, ShaderParam>(name, ShaderParam(EumnTypeName, &m_value))); \
        }                                                                                                            \
                                                                                                                     \
        operator BaseTypeName() const                                                                                \
        {                                                                                                            \
            return m_value;                                                                                          \
        }                                                                                                            \
                                                                                                                     \
        RegTypeName &operator=(BaseTypeName value)                                                                   \
        {                                                                                                            \
            m_value = value;                                                                                         \
            return *this;                                                                                            \
        }                                                                                                            \
                                                                                                                     \
        BaseTypeName &operator()()                                                                                   \
        {                                                                                                            \
            return m_value;                                                                                          \
        }                                                                                                            \
                                                                                                                     \
        BaseTypeName &get()                                                                                          \
        {                                                                                                            \
            return m_value;                                                                                          \
        }                                                                                                            \
                                                                                                                     \
    private:                                                                                                         \
        BaseTypeName m_value;                                                                                        \
    };

SHADER_TYPE_REG(int, ShaderParam_Type::Int, SDInt)

SHADER_TYPE_REG(float, ShaderParam_Type::Float, SDFloat)
SHADER_TYPE_REG(glm::vec2, ShaderParam_Type::Vec2, SDVec2)
SHADER_TYPE_REG(glm::vec3, ShaderParam_Type::Vec3, SDVec3)
SHADER_TYPE_REG(glm::vec4, ShaderParam_Type::Vec4, SDVec4)

SHADER_TYPE_REG(glm::mat2, ShaderParam_Type::Mat2, SDMat2)
SHADER_TYPE_REG(glm::mat3, ShaderParam_Type::Mat3, SDMat3)
SHADER_TYPE_REG(glm::mat4, ShaderParam_Type::Mat4, SDMat4)

SHADER_TYPE_REG(Ref<Texture2D>, ShaderParam_Type::Texture2D, SDTexture2D)
SHADER_TYPE_REG(Ref<TextureCube>, ShaderParam_Type::TextureCube, SDTextureCube)

#define PTR_AS(type, ptr) *((type *)ptr)