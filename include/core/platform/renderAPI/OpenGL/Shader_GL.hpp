#pragma once

#include "core/platform/renderAPI/Shader.hpp"
#include "scene/ShaderManager.hpp"

#include <glm/glm.hpp>

void shader_error_check(unsigned int shader, Shader_Type type);
void pipeline_error_check(unsigned int pipeline);

class Shader_GL : public Shader
{
public:
    Shader_GL(const std::string &path, Shader_Type type) : Shader(path, type), m_compiled(false), m_id(0) {}

    Shader_GL():Shader(){}

    void read_file();

    void compiled();

    virtual ~Shader_GL();

    unsigned int get_id();

    bool is_compiled() { return m_compiled; }

private:
    bool m_compiled;
    std::string m_code;
    unsigned int m_id;
};

class Pipeline_GL : public Pipeline
{
public:
    Pipeline_GL() : m_id(0), m_texture_index(0)
    {
    }

    virtual void bind() override;

    virtual void set_params(ShaderParamList &params) override;

    virtual void set_params(const std::string &name, ShaderParam &param) override;

    unsigned int get_id()
    {
        return m_id;
    }

private:
    unsigned int m_id;
    int m_texture_index;
    std::unordered_map<std::string, int> m_params_map;
};

#define DECLARE_SHADER(TypeName)                                                \
    TypeName(const std::string &path, Shader_Type type) : Shader_GL(path, type) \
    {                                                                           \
        ShaderManager::register_shader(m_path, &instance);                      \
    }                                                                           \
    static TypeName instance;                                                   \
    virtual ShaderParamList &get_params() override { return pms; }              \
                                                                                \
private:                                                                        \
    Parameters pms;

void shader_error_check(unsigned int shader, Shader_Type type);

void pipeline_error_check(unsigned int pipeline);