#include "core/platform/renderAPI/OpenGL/Shader_GL.hpp"
#include "core/platform/renderAPI/OpenGL/Texture_GL.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>

Shader_GL::~Shader_GL()
{
    if (m_compiled)
        glDeleteShader(m_id);
}

void Shader_GL::read_file()
{
    m_code = read_from_file(m_path);
}

void Shader_GL::compiled()
{
    if (m_code.size() == 0)
    {
        read_file();
    }

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
    default:
        break;
    }

    const char *code = m_code.c_str();

    glShaderSource(m_id, 1, &code, NULL);
    glCompileShader(m_id);
    shader_error_check(m_id, m_type);
    m_compiled = true;
}

unsigned int Shader_GL::get_id()
{
    return m_id;
}

void Pipline_GL::bind()
{
    if (m_id == 0)
    {
        m_id = glCreateProgram();

        for (auto &i : m_shaders)
        {
            Shader_GL &shader = *(Shader_GL *)i;
            if (!shader.is_compiled())
                shader.compiled();
            glAttachShader(m_id, shader.get_id());
        }

        glLinkProgram(m_id);
        pipeline_error_check(m_id);
    }

    m_texture_index = 0;
    glUseProgram(m_id);
}

void Pipline_GL::set_params(const std::string &name, ShaderParam &param)
{
    auto loc = m_params_map.find(name);
    if (loc == m_params_map.end())
    {
        auto newloc = glGetUniformLocation(m_id, name.c_str());
        loc = m_params_map.insert(std::pair<std::string,int>(name, newloc)).first;
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

    default:
        break;
    }
}

void Pipline_GL::set_params(ShaderParamList &params)
{
    for (auto &i : params.m_param_list)
        Pipline_GL::set_params(i.first, i.second);
}

void shader_error_check(unsigned int shader, Shader_Type type)
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
}

void pipeline_error_check(unsigned int pipeline)
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
}