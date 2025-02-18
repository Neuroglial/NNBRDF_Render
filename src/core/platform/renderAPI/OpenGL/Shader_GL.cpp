// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#include "core/platform/renderAPI/OpenGL/Shader_GL.hpp"
#include "core/platform/renderAPI/OpenGL/Texture_GL.hpp"
#include "core/platform/renderAPI/RenderAPI.hpp"
#include "core/platform/renderAPI/OpenGL/CheckTool.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>

REGISTER_API(Shader_GL);
REGISTER_API(Pipeline_GL)

bool shader_error_check(unsigned int shader, Shader_Type type);
bool pipeline_error_check(unsigned int pipeline);

Shader_GL::~Shader_GL()
{
    if (m_compiled)
        glDeleteShader(m_id);
}

bool Shader_GL::compile()
{
    if (m_id != 0)
        glDeleteShader(m_id);

    switch (m_type)
    {
    case Shader_Type::VERTEX_SHADER:
        m_id = glCreateShader(GL_VERTEX_SHADER);
        break;
    case Shader_Type::FRAGMENT_SHADER:
        m_id = glCreateShader(GL_FRAGMENT_SHADER);
        break;
    case Shader_Type::GEOMETRY_SHADER:
        m_id = glCreateShader(GL_GEOMETRY_SHADER);
        break;
    default:
        break;
    }

    const char *code = m_code.c_str();

    glShaderSource(m_id, 1, &code, NULL);
    glCompileShader(m_id);
    m_compiled = shader_error_check(m_id, m_type);

    if (!m_compiled)
    {
        Log(m_code);
    }

    return m_compiled;
}

unsigned int Shader_GL::get_id()
{
    return m_id;
}

bool Pipeline_GL::compile()
{
    if (m_id)
        glDeleteProgram(m_id);

    m_id = glCreateProgram();

    for (auto &i : m_shaders)
    {
        Shader_GL &shader = *(Shader_GL *)i.get();
        if (!shader.is_compiled())
            shader.compile();
        glAttachShader(m_id, shader.get_id());
    }

    glLinkProgram(m_id);

    return pipeline_error_check(m_id);
}

void Pipeline_GL::clear()
{
    m_shaders.clear();
    m_params_map.clear();
}

void Pipeline_GL::bind()
{
    if (m_id == 0)
        compile();

    m_texture_index = 0;
    glUseProgram(m_id);
}

Pipeline_GL::~Pipeline_GL()
{
    if (m_id)
        glDeleteProgram(m_id);
}

Ref<ShaderParamList> Pipeline_GL::get_params_list()
{
    Ref<ShaderParamList> ret(new ShaderParamList);
    if (m_id == 0)
        bind();

    int uniformCount;
    glGetProgramiv(m_id, GL_ACTIVE_UNIFORMS, &uniformCount);

    auto get_type = [](GLenum type) -> ShaderParam_Type
    {
        switch (type)
        {
        case GL_INT:
            return ShaderParam_Type::Int;
        case GL_FLOAT:
            return ShaderParam_Type::Float;
        case GL_FLOAT_VEC2:
            return ShaderParam_Type::Vec2;
        case GL_FLOAT_VEC3:
            return ShaderParam_Type::Vec3;
        case GL_FLOAT_VEC4:
            return ShaderParam_Type::Vec4;
        case GL_FLOAT_MAT2:
            return ShaderParam_Type::Mat2;
        case GL_FLOAT_MAT3:
            return ShaderParam_Type::Mat3;
        case GL_FLOAT_MAT4:
            return ShaderParam_Type::Mat4;
        case GL_SAMPLER_2D:
            return ShaderParam_Type::Texture2D;
        case GL_SAMPLER_CUBE:
            return ShaderParam_Type::TextureCube;

        default:
            return ShaderParam_Type::None;
        }
    };

    std::string str;
    str.reserve(512);

    for (GLint i = 0; i < uniformCount; ++i)
    {
        char name[256];
        GLsizei length;
        GLint size;
        GLenum type;
        glGetActiveUniform(m_id, i, sizeof(name), &length, &size, &type, name);
        if (name[0] == 'u' && name[1] == 'b' && name[2] == '_')
            continue;

        auto loc = glGetUniformLocation(m_id, name);
        if (loc >= 0)
        {
            m_params_map.insert(std::pair<std::string, int>(name, loc));
            ret->m_param_list.emplace(name, get_type(type));
        }

        str = name;

        if (str.find("[0]") != std::string::npos)
        {
            str = str.substr(0, str.size() - 2);
            for (int i = 1; 1; ++i)
            {
                std::string name_t = str + std::to_string(i) + "]";
                loc = glGetUniformLocation(m_id, name_t.c_str());
                if (loc >= 0)
                {
                    m_params_map.insert(std::pair<std::string, int>(name_t, loc));
                    ret->m_param_list.emplace(name_t, get_type(type));
                }
                else
                    break;
            }
        }
    }

    return ret;
}

void Pipeline_GL::set_params(const std::string &name, ShaderParam &param)
{
    if (!param.changed)
        return;

    auto loc = m_params_map.find(name);
    if (loc == m_params_map.end())
    {
        auto newloc = glGetUniformLocation(m_id, name.c_str());
        loc = m_params_map.insert(std::pair<std::string, int>(name, newloc)).first;
    }

    switch (param.m_type)
    {
    case ShaderParam_Type::Float:
        glUniform1fv(loc->second, 1, (float *)param.m_value_ptr);
        break;

    case ShaderParam_Type::Vec2:
        glUniform2fv(loc->second, 1, (float *)param.m_value_ptr);
        break;
    case ShaderParam_Type::Vec3:
        glUniform3fv(loc->second, 1, (float *)param.m_value_ptr);
        break;
    case ShaderParam_Type::Vec4:
        glUniform4fv(loc->second, 1, (float *)param.m_value_ptr);
        break;

    case ShaderParam_Type::Int:
        glUniform1iv(loc->second, 1, (int *)param.m_value_ptr);
        break;

    case ShaderParam_Type::Mat2:
    {
        auto &mat = PTR_AS(glm::mat2, param.m_value_ptr);
        glUniformMatrix2fv(loc->second, 1, GL_FALSE, &mat[0][0]);
        break;
    }

    case ShaderParam_Type::Mat3:
    {
        auto &mat = PTR_AS(glm::mat3, param.m_value_ptr);
        glUniformMatrix3fv(loc->second, 1, GL_FALSE, &mat[0][0]);
        break;
    }

    case ShaderParam_Type::Mat4:
    {
        auto &mat = PTR_AS(glm::mat4, param.m_value_ptr);
        glUniformMatrix4fv(loc->second, 1, GL_FALSE, &mat[0][0]);
        break;
    }

    case ShaderParam_Type::Texture2D:
    {
        auto &tex1 = PTR_AS(Ref<Texture2D_GL>, param.m_value_ptr);
        if (tex1 == nullptr)
            break;
        glUniform1iv(loc->second, 1, &m_texture_index);
        glActiveTexture(GL_TEXTURE0 + m_texture_index++);
        glBindTexture(GL_TEXTURE_2D, tex1->get_id());
        break;
    }

    case ShaderParam_Type::TextureCube:
    {
        auto &tex1 = PTR_AS(Ref<TextureCube_GL>, param.m_value_ptr);
        if (tex1 == nullptr)
            break;
        glUniform1iv(loc->second, 1, &m_texture_index);
        glActiveTexture(GL_TEXTURE0 + m_texture_index++);
        // glBindTexture(GL_TEXTURE_2D, 0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, tex1->get_id());
        GL_Check();
        break;
    }

    default:
        break;
    }

    GL_Check()
}

void Pipeline_GL::set_params(ShaderParamList &params)
{
    for (auto &i : params.m_param_list)
        Pipeline_GL::set_params(i.first, i.second);
}

bool shader_error_check(unsigned int shader, Shader_Type type)
{
    int success;
    char infoLog[1024];

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 1024, NULL, infoLog);
        std::cout << "ERROR::SHADER_COMPILATION_ERROR:\n"
                  << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
    }

    return success;
}

bool pipeline_error_check(unsigned int pipeline)
{
    int success;
    char infoLog[1024];

    glGetProgramiv(pipeline, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(pipeline, 1024, NULL, infoLog);
        std::cout << "ERROR::PROGRAM_LINKING_ERROR of type:\n"
                  << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
    }

    return success;
}
