#include "core/platform/renderAPI/OpenGL/Shader_GL.hpp"
#include "core/platform/renderAPI/OpenGL/Texture_GL.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Shader
{
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

    void Pipline_GL::set_params(ParamList &params)
    {
        for (auto &i : params.m_param_list)
        {
            switch (i.second.m_type)
            {
            case Param_Type::Float:
                glUniform1fv(glGetUniformLocation(m_id, i.first.c_str()), 1, (float *)i.second.m_value_ptr);
                break;

            case Param_Type::Vec2:
                glUniform2fv(glGetUniformLocation(m_id, i.first.c_str()), 1, (float *)i.second.m_value_ptr);
                break;
            case Param_Type::Vec3:
                glUniform3fv(glGetUniformLocation(m_id, i.first.c_str()), 1, (float *)i.second.m_value_ptr);
                break;
            case Param_Type::Vec4:
                glUniform4fv(glGetUniformLocation(m_id, i.first.c_str()), 1, (float *)i.second.m_value_ptr);
                break;

            case Param_Type::Int:
                glUniform1iv(glGetUniformLocation(m_id, i.first.c_str()), 1, (int *)i.second.m_value_ptr);
                break;

            case Param_Type::Mat2:
                glUniformMatrix2fv(glGetUniformLocation(m_id, i.first.c_str()), 1,GL_FALSE,(float *)i.second.m_value_ptr);
                break;

            case Param_Type::Mat3:
                glUniformMatrix3fv(glGetUniformLocation(m_id, i.first.c_str()), 1,GL_FALSE,(float *)i.second.m_value_ptr);
                break;

            case Param_Type::Mat4:
                glUniformMatrix4fv(glGetUniformLocation(m_id, i.first.c_str()), 1,GL_FALSE,(float *)i.second.m_value_ptr);
                break;

            case Param_Type::Texture2D:{
                auto& tex1 = PTR_AS(Ref<Texture::Texture2D_GL>,i.second.m_value_ptr);
                if(tex1==nullptr)
                    break;
                glUniform1iv(glGetUniformLocation(m_id, i.first.c_str()), 1, &m_texture_index);
                glActiveTexture(GL_TEXTURE0 + m_texture_index++);
                glBindTexture(GL_TEXTURE_2D, tex1->get_id());
                break;
            }
        
            default:
                break;
            }
        }
    }

} // namespace Shader

void shader_error_check(unsigned int shader, Shader::Shader_Type type)
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