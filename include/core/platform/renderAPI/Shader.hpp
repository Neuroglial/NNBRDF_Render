#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <glm/glm.hpp>
#include "utils/utils.hpp"
#include "core/platform/renderAPI/ShaderParam.hpp"

enum class Shader_Type
{
    VERTEX_SHADER,
    FRAGMENT_SHADER
};

class Shader
{
public:
    Shader(const std::string &path, Shader_Type type) : m_path(path), m_type(type) {}
    virtual ~Shader()
    {
    }

    virtual ShaderParamList &get_params() = 0;

protected:
    std::string m_path;
    Shader_Type m_type;
};

class Pipline
{
public:
    void attach_shader(Shader *shader)
    {
        m_shaders.push_back(shader);
    }

    virtual void set_params(ShaderParamList &params) = 0;
    virtual void set_params(const std::string &name, ShaderParam &param) = 0;

    virtual void bind() = 0;

    virtual ~Pipline()
    {
    }

protected:
    std::vector<Shader *> m_shaders;
};
