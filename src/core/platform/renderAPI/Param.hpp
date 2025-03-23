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

enum class ParamType
{
    None = 0,
    Int = 1,
    Float = 2,
    Vec2 = 3,
    Vec3 = 4,
    Vec4 = 5,
    Mat2 = 6,
    Mat3 = 7,
    Mat4 = 8,
    Texture2D = 9,
    TextureCube = 10
};

#define AS(type, ptr) *((type *)ptr)

class Param;

using ParamDic = std::map<std::string, Param *>;

class Params
{
    std::vector<Ref<Param>> ref_Params;

public:
    ParamDic m_list;

    Param *operator[](const std::string &name)
    {
        auto &i = m_list.find(name);
        if (i != m_list.end())
            return i->second;

        return nullptr;
    }

    const Param *operator[](const std::string &name) const
    {
        auto &i = m_list.find(name);
        if (i != m_list.end())
            return i->second;

        return nullptr;
    }

    void get(const Params &other);
    void add(Ref<Param> param);
    void add(Param *param);
    Ref<Params> copy();
};

class Param
{
protected:
    ParamType m_type;
    std::string m_name;
    bool m_changed;

public:
    static std::map<ParamType, std::function<Param *(const std::string &)>> &get_creator()
    {
        static std::map<ParamType, std::function<Param *(const std::string &)>> ctr;
        return ctr;
    }

    Param(ParamType type, const std::string &name, Params *list) : m_type(type), m_name(name), m_changed(false)
    {
        if (list)
            list->m_list.emplace(m_name, this);
    }

    const bool changed() const
    {
        return m_changed;
    }

    bool set_changed(bool changed)
    {
        bool last = m_changed;
        m_changed = changed;

        return last;
    }

    virtual void set(void *ptr) = 0;

    ParamType type() const
    {
        return m_type;
    }

    virtual bool set(const Param &other) = 0;

    Param &operator=(const Param &other)
    {
        set(other);
        return *this;
    }

    const std::string &name() const
    {
        return m_name;
    }

    template <typename T>
    T *as()
    {
        return dynamic_cast<T *>(this);
    }

    template <typename T>
    const T *as() const
    {
        auto p = const_cast<Param *>(this);
        return dynamic_cast<T *>(p);
    }

    virtual Param *copy() = 0;
};

#define PARAMS Params m_Params
#define PMV(Type, name, value) Type name = Type(#name, value, &m_Params)
#define PM(Type, name) Type name = Type(#name, &m_Params)

#define REG_PARAM_TYPE(Type, TypeEume)                                                                                                                    \
    class PM_##TypeEume : public Param                                                                                                                    \
    {                                                                                                                                                     \
    private:                                                                                                                                              \
        Type m_value;                                                                                                                                     \
                                                                                                                                                          \
    public:                                                                                                                                               \
        PM_##TypeEume(const std::string &name, Params *list = nullptr) : Param(ParamType::TypeEume, name, list) {}                                        \
        PM_##TypeEume(const std::string &name, Type v, Params *list = nullptr) : Param(ParamType::TypeEume, name, list), m_value(v) { m_changed = true; } \
                                                                                                                                                          \
        operator Type()                                                                                                                                   \
        {                                                                                                                                                 \
            return m_value;                                                                                                                               \
        }                                                                                                                                                 \
                                                                                                                                                          \
        Type &get()                                                                                                                                       \
        {                                                                                                                                                 \
            return m_value;                                                                                                                               \
        }                                                                                                                                                 \
                                                                                                                                                          \
        const Type &get() const                                                                                                                           \
        {                                                                                                                                                 \
            return m_value;                                                                                                                               \
        }                                                                                                                                                 \
                                                                                                                                                          \
        virtual bool set(const Param &other) override                                                                                                     \
        {                                                                                                                                                 \
            if (other.type() != this->type())                                                                                                             \
                return false;                                                                                                                             \
            auto *ptr = other.as<PM_##TypeEume>();                                                                                                        \
            *this = *ptr;                                                                                                                                 \
            return true;                                                                                                                                  \
        }                                                                                                                                                 \
                                                                                                                                                          \
        PM_##TypeEume &operator=(const Type &v)                                                                                                           \
        {                                                                                                                                                 \
            m_changed = true;                                                                                                                             \
            m_value = v;                                                                                                                                  \
            return *this;                                                                                                                                 \
        }                                                                                                                                                 \
                                                                                                                                                          \
        PM_##TypeEume &operator=(const PM_##TypeEume &v)                                                                                                  \
        {                                                                                                                                                 \
            m_changed = v.changed();                                                                                                                      \
            m_value = v.m_value;                                                                                                                          \
            return *this;                                                                                                                                 \
        }                                                                                                                                                 \
                                                                                                                                                          \
        void set(void *ptr) override                                                                                                                      \
        {                                                                                                                                                 \
            m_changed = true;                                                                                                                             \
            m_value = *((Type *)ptr);                                                                                                                     \
        }                                                                                                                                                 \
                                                                                                                                                          \
        virtual Param *copy() override                                                                                                                    \
        {                                                                                                                                                 \
            return new PM_##TypeEume(m_name, m_value);                                                                                                    \
        }                                                                                                                                                 \
    };                                                                                                                                                    \
    struct PMREG_##TypeEume                                                                                                                               \
    {                                                                                                                                                     \
        PMREG_##TypeEume()                                                                                                                                \
        {                                                                                                                                                 \
            Param::get_creator().emplace(ParamType::TypeEume, [](const std::string &name) -> Param * { return new PM_##TypeEume(name); });                \
        }                                                                                                                                                 \
    };                                                                                                                                                    \
    inline PMREG_##TypeEume PMREG_##TypeEume##Instance;

REG_PARAM_TYPE(float, Float)
REG_PARAM_TYPE(int, Int)
REG_PARAM_TYPE(glm::vec2, Vec2)
REG_PARAM_TYPE(glm::vec3, Vec3)
REG_PARAM_TYPE(glm::vec4, Vec4)
REG_PARAM_TYPE(glm::mat2, Mat2)
REG_PARAM_TYPE(glm::mat3, Mat3)
REG_PARAM_TYPE(glm::mat4, Mat4)
REG_PARAM_TYPE(Ref<Texture2D>, Texture2D)
REG_PARAM_TYPE(Ref<TextureCube>, TextureCube)

// When adding types, pay attention to adding get_type() in Helper and Shader_GL;

struct ParamHelper
{
    static std::string to_string(ParamType type);
    static ParamType to_type(const std::string &type);
    static void *alloc(ParamType type);
    static void del(void *ptr, ParamType type);
    static void set(void *ptr_d, void *const ptr_s, ParamType type);
};

// struct SD_Param
// {
//     ParamType m_type;
//     void *m_value_ptr;
//     bool changed = false;

//     SD_Param(ParamType type, void *ptr) : m_type(type), m_value_ptr(ptr), m_alloc(false)
//     {
//     }

//     SD_Param(ParamType type) : m_type(type), m_alloc(true)
//     {
//         m_value_ptr = ParamHelper::alloc(m_type);
//     }

//     SD_Param(SD_Param &&other) noexcept
//     {
//         m_type = other.m_type;
//         m_alloc = other.m_alloc;
//         m_value_ptr = other.m_value_ptr;

//         other.m_alloc = false;
//         other.m_value_ptr = nullptr;
//         other.m_type = ParamType::None;
//     }

//     SD_Param(const SD_Param &other) noexcept
//     {
//         m_type = other.m_type;
//         m_alloc = true;
//         m_value_ptr = ParamHelper::alloc(m_type);
//     }

//     ~SD_Param()
//     {
//         if (m_alloc && m_value_ptr)
//             ParamHelper::del(m_value_ptr, m_type);
//     }

//     void set(void *value)
//     {
//         changed = true;
//         ParamHelper::set(m_value_ptr, value, m_type);
//     }

// private:
//     bool m_alloc;
// };

// struct SD_ParamList
// {
//     std::map<std::string, SD_Param> m_param_list;

//     SD_ParamList() = default;

//     SD_ParamList(SD_ParamList &&other)
//     {
//         m_param_list.swap(other.m_param_list);
//     }

//     SD_ParamList(const SD_ParamList &other)
//     {
//         *this = other;
//     }

//     SD_ParamList &operator=(const SD_ParamList &other)
//     {
//         m_param_list.clear();
//         for (auto &i : other.m_param_list)
//         {
//             m_param_list.insert(std::pair<std::string, SD_Param>(i.first, SD_Param(i.second.m_type)));
//         }

//         return *this;
//     }

//     SD_ParamList &operator=(SD_ParamList &&other)
//     {
//         m_param_list.swap(other.m_param_list);
//         return *this;
//     }

//     SD_ParamList operator+(const SD_ParamList &other)
//     {
//         SD_ParamList ret;

//         ret = *this;
//         for (auto &i : other.m_param_list)
//         {
//             ret.m_param_list.insert(std::pair<std::string, SD_Param>(i.first, SD_Param(i.second.m_type)));
//         }

//         return ret;
//     }

//     SD_ParamList &operator+=(const SD_ParamList &other)
//     {
//         *this = *this + other;
//         return *this;
//     }

//     SD_Param *operator[](const std::string &param_name)
//     {
//         auto i = m_param_list.find(param_name);
//         if (i != m_param_list.end())
//         {
//             return &i->second;
//         }
//         else
//         {
//             // Log("Shader Parameter Named " + param_name + " Don't Exist");
//             return nullptr;
//         }
//     }

//     std::string to_string()
//     {
//         std::string ret;
//         for (auto &i : m_param_list)
//         {
//             ret += (ParamHelper::to_string(i.second.m_type) + " " + i.first) + "\n";
//         }

//         return ret;
//     };
// };

// #define TYPE_REG(BaseTypeName, EumnTypeName, RegTypeName)                                                          \
//     struct RegTypeName                                                                                             \
//     {                                                                                                              \
//         operator BaseTypeName()                                                                                    \
//         {                                                                                                          \
//             return value;                                                                                          \
//         }                                                                                                          \
//         RegTypeName(SD_ParamList *pl, const std::string &name)                                                     \
//         {                                                                                                          \
//             pl->m_param_list.insert(std::pair<std::string, SD_Param>(name, SD_Param(EumnTypeName, &value)));       \
//         }                                                                                                          \
//                                                                                                                    \
//     private:                                                                                                       \
//         BaseTypeName value;                                                                                        \
//     };                                                                                                             \
//     struct RegTypeName##_Array                                                                                     \
//     {                                                                                                              \
//         RegTypeName##_Array(SD_ParamList *pl, const std::string &name, int size)                                   \
//         {                                                                                                          \
//             value = new BaseTypeName[size];                                                                        \
//             for (int i = 0; i < size; ++i)                                                                         \
//             {                                                                                                      \
//                 pl->m_param_list.emplace(name + "[" + std::to_string(i) + "]", SD_Param(EumnTypeName, &value[i])); \
//             }                                                                                                      \
//         }                                                                                                          \
//         ~RegTypeName##_Array()                                                                                     \
//         {                                                                                                          \
//             delete[] value;                                                                                        \
//         }                                                                                                          \
//                                                                                                                    \
//     private:                                                                                                       \
//         BaseTypeName *value;                                                                                       \
//     };

// TYPE_REG(int, ParamType::Int, SDInt)

// TYPE_REG(float, ParamType::Float, SDFloat)
// TYPE_REG(glm::vec2, ParamType::Vec2, SDVec2)
// TYPE_REG(glm::vec3, ParamType::Vec3, SDVec3)
// TYPE_REG(glm::vec4, ParamType::Vec4, SDVec4)
// TYPE_REG(glm::mat2, ParamType::Mat2, SDMat2)
// TYPE_REG(glm::mat3, ParamType::Mat3, SDMat3)
// TYPE_REG(glm::mat4, ParamType::Mat4, SDMat4)

// TYPE_REG(Ref<Texture2D>, ParamType::Texture2D, SDTexture2D)
// TYPE_REG(Ref<TextureCube>, ParamType::TextureCube, SDTextureCube)

// #define PARAM_LIST SD_ParamList m_ParamList;

// #define SHADER_PARAM(TypeName, Name) TypeName Name = TypeName(this->m_ParamList, #Name);
// #define SHADER_PARAM_ARRAY(TypeName, Name, Size) TypeName##_Array Name = TypeName##_Array(this->m_ParamList, #Name, Size);