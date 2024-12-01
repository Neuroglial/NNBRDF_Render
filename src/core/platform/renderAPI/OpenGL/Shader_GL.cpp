#include "core/platform/renderAPI/OpenGL/Shader_GL.hpp"

#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Shader
{

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
                if (!shader.m_compiled)
                    shader.compiled();
                glAttachShader(m_id, shader.get_id());
            }

            glLinkProgram(m_id);
            pipeline_error_check(m_id);
        }

        glUseProgram(m_id);
    }

    void Pipline_GL::set_params(ParamList &params)
    {
        for (auto &i : params.m_param_list)
        {
            switch (i.second.m_type)
            {
            case Param_Type::Float:
                glUniform1f(glGetUniformLocation(m_id, i.first.c_str()),
                            *(float *)i.second.m_value_ptr);
                break;
            case Param_Type::Vec2:
                glUniform2f(glGetUniformLocation(m_id, i.first.c_str()),
                            (*(glm::vec2 *)i.second.m_value_ptr).x,
                            (*(glm::vec2 *)i.second.m_value_ptr).y);
                break;
            case Param_Type::Vec3:
                glUniform3f(glGetUniformLocation(m_id, i.first.c_str()),
                            (*(glm::vec3 *)i.second.m_value_ptr).x,
                            (*(glm::vec3 *)i.second.m_value_ptr).y,
                            (*(glm::vec3 *)i.second.m_value_ptr).z);

                break;
            case Param_Type::Vec4:
                glUniform4f(glGetUniformLocation(m_id, i.first.c_str()),
                            (*(glm::vec4 *)i.second.m_value_ptr).x,
                            (*(glm::vec4 *)i.second.m_value_ptr).y,
                            (*(glm::vec4 *)i.second.m_value_ptr).z,
                            (*(glm::vec4 *)i.second.m_value_ptr).w);
                break;

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