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

#include "nlohmann/json.hpp"

enum class Param_Type
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

// When adding types, pay attention to adding get_type() in Helper and Shader_GL;

struct ShaderParam_Helper
{
    static std::string to_string(Param_Type type);
    static void *alloc(Param_Type type);
    static void del(void *ptr, Param_Type type);
    static void set(void *ptr_d, void *const ptr_s, Param_Type type);
};

struct Param
{
    Param_Type m_type;
    void *m_value_ptr;
    bool changed = false;

    Param(Param_Type type, void *ptr) : m_type(type), m_value_ptr(ptr), m_alloc(false)
    {
    }

    Param(Param_Type type) : m_type(type), m_alloc(true)
    {
        m_value_ptr = ShaderParam_Helper::alloc(m_type);
    }

    Param(Param &&other) noexcept
    {
        m_type = other.m_type;
        m_alloc = other.m_alloc;
        m_value_ptr = other.m_value_ptr;

        other.m_alloc = false;
        other.m_value_ptr = nullptr;
        other.m_type = Param_Type::None;
    }

    Param(const Param &other) noexcept
    {
        m_type = other.m_type;
        m_alloc = true;
        m_value_ptr = ShaderParam_Helper::alloc(m_type);
    }

    ~Param()
    {
        if (m_alloc && m_value_ptr)
            ShaderParam_Helper::del(m_value_ptr, m_type);
    }

    void set(void *value)
    {
        changed = true;
        ShaderParam_Helper::set(m_value_ptr, value, m_type);
    }

private:
    bool m_alloc;
};

struct ParamList
{
    std::map<std::string, Param> m_param_list;

    ParamList() = default;

    ParamList(ParamList &&other)
    {
        m_param_list.swap(other.m_param_list);
    }

    ParamList(const ParamList &other)
    {
        *this = other;
    }

    ParamList &operator=(const ParamList &other)
    {
        m_param_list.clear();
        for (auto &i : other.m_param_list)
        {
            m_param_list.insert(std::pair<std::string, Param>(i.first, Param(i.second.m_type)));
        }

        return *this;
    }

    ParamList &operator=(ParamList &&other)
    {
        m_param_list.swap(other.m_param_list);
        return *this;
    }

    ParamList operator+(const ParamList &other)
    {
        ParamList ret;

        ret = *this;
        for (auto &i : other.m_param_list)
        {
            ret.m_param_list.insert(std::pair<std::string, Param>(i.first, Param(i.second.m_type)));
        }

        return ret;
    }

    ParamList &operator+=(const ParamList &other)
    {
        *this = *this + other;
        return *this;
    }

    Param *operator[](const std::string &param_name)
    {
        auto i = m_param_list.find(param_name);
        if (i != m_param_list.end())
        {
            return &i->second;
        }
        else
        {
            // Log("Shader Parameter Named " + param_name + " Don't Exist");
            return nullptr;
        }
    }

    std::string to_string()
    {
        std::string ret;
        for (auto &i : m_param_list)
        {
            ret += (ShaderParam_Helper::to_string(i.second.m_type) + " " + i.first) + "\n";
        }

        return ret;
    };
};

#define TYPE_REG(BaseTypeName, EumnTypeName, RegTypeName)                                                       \
    struct RegTypeName                                                                                          \
    {                                                                                                           \
        operator BaseTypeName()                                                                                 \
        {                                                                                                       \
            return value;                                                                                       \
        }                                                                                                       \
        RegTypeName(ParamList *pl, const std::string &name)                                                     \
        {                                                                                                       \
            pl->m_param_list.insert(std::pair<std::string, Param>(name, Param(EumnTypeName, &value)));          \
        }                                                                                                       \
                                                                                                                \
    private:                                                                                                    \
        BaseTypeName value;                                                                                     \
    };                                                                                                          \
    struct RegTypeName##_Array                                                                                  \
    {                                                                                                           \
        RegTypeName##_Array(ParamList *pl, const std::string &name, int size)                                   \
        {                                                                                                       \
            value = new BaseTypeName[size];                                                                     \
            for (int i = 0; i < size; ++i)                                                                      \
            {                                                                                                   \
                pl->m_param_list.emplace(name + "[" + std::to_string(i) + "]", Param(EumnTypeName, &value[i])); \
            }                                                                                                   \
        }                                                                                                       \
        ~RegTypeName##_Array()                                                                                  \
        {                                                                                                       \
            delete[] value;                                                                                     \
        }                                                                                                       \
                                                                                                                \
    private:                                                                                                    \
        BaseTypeName *value;                                                                                    \
    };

TYPE_REG(int, Param_Type::Int, SDInt)

TYPE_REG(float, Param_Type::Float, SDFloat)
TYPE_REG(glm::vec2, Param_Type::Vec2, SDVec2)
TYPE_REG(glm::vec3, Param_Type::Vec3, SDVec3)
TYPE_REG(glm::vec4, Param_Type::Vec4, SDVec4)
TYPE_REG(glm::mat2, Param_Type::Mat2, SDMat2)
TYPE_REG(glm::mat3, Param_Type::Mat3, SDMat3)
TYPE_REG(glm::mat4, Param_Type::Mat4, SDMat4)

TYPE_REG(Ref<Texture2D>, Param_Type::Texture2D, SDTexture2D)
TYPE_REG(Ref<TextureCube>, Param_Type::TextureCube, SDTextureCube)

#define PARAM_LIST ParamList m_ParamList;

#define SHADER_PARAM(TypeName, Name) TypeName Name = TypeName(this->m_ParamList, #Name);
#define SHADER_PARAM_ARRAY(TypeName, Name, Size) TypeName##_Array Name = TypeName##_Array(this->m_ParamList, #Name, Size);

#define PTR_AS(type, ptr) *((type *)ptr)