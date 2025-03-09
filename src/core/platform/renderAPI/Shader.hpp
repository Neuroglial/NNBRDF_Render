// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "utils/utils.hpp"
#include "core/platform/renderAPI/Param.hpp"

enum class Shader_Type
{
    NONE,
    VERTEX_SHADER,
    GEOMETRY_SHADER,
    FRAGMENT_SHADER
};

class Shader
{
public:
    Shader(const std::string &code, Shader_Type type) : m_code(code), m_type(type) {}

    Shader() : m_type(Shader_Type::NONE) {}

    void set(const std::string &code, Shader_Type type)
    {
        m_code = code;
        m_type = type;
        m_compiled = false;
    }

    virtual bool compile() = 0;

    virtual ~Shader()
    {
    }

    ParamList &get_params()
    {
        if (m_pms_list == nullptr)
            throw std::runtime_error("ParamsList Dont Exist");
        return *m_pms_list;
    }

    void set_params_list(ParamList *list)
    {
        m_pms_list = list;
    }

protected:
    ParamList *m_pms_list;
    std::string m_code;
    Shader_Type m_type;
    bool m_compiled = false;
};

class Pipeline
{
public:
    void attach_shader(Ref<Shader> &shader)
    {
        m_shaders.push_back(shader);
    }

    virtual void set_params(ParamList &params) = 0;
    virtual void set_params(const std::string &name, Param &param) = 0;
    virtual Ref<ParamList> get_params_list() = 0;

    virtual void bind() = 0;
    virtual bool compile() = 0;
    virtual void clear() = 0;

    virtual ~Pipeline()
    {
    }

    std::vector<Ref<Shader>> m_shaders;
};
