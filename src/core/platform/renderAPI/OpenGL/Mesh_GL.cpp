#include "core/platform/renderAPI/OpenGL/Mesh_GL.hpp"
#include "core/platform/renderAPI/RenderAPI.hpp"

REGISTER_API(Mesh_GL)

void Mesh_GL::bind()
{
    if (!m_binded)
    {
        if (m_VAO)
            glDeleteVertexArrays(1, &m_VAO);

        glGenVertexArrays(1, &m_VAO);
        glBindVertexArray(m_VAO);
        
        auto m_VBO = dynamic_cast<ArrayBuffer_GL<float> *>(m_vertex_buffer.get());
        auto m_EBO = dynamic_cast<ArrayBuffer_GL<unsigned int> *>(m_element_buffer.get());

        if(!m_VBO)
            throw std::runtime_error("Vertex Buffer Error");

        glBindBuffer(GL_ARRAY_BUFFER, m_VBO->get_id());
        if (m_EBO)
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO->get_id());

        long long ptr = 0;
        int index = 0;

        for (auto i : m_layout)
        {
            switch (i)
            {
            case ShaderParam_Type::Float:
                glVertexAttribPointer(index, 1, GL_FLOAT, GL_FALSE, m_strike, (void *)ptr);
                glEnableVertexAttribArray(index++);
                ptr += sizeof(float);
                break;
            case ShaderParam_Type::Vec2:
                glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE, m_strike, (void *)ptr);
                glEnableVertexAttribArray(index++);
                ptr += sizeof(float) * 2;
                break;
            case ShaderParam_Type::Vec3:
                glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE, m_strike, (void *)ptr);
                glEnableVertexAttribArray(index++);
                ptr += sizeof(float) * 3;
                break;
            case ShaderParam_Type::Vec4:
                glVertexAttribPointer(index, 4, GL_FLOAT, GL_FALSE, m_strike, (void *)ptr);
                glEnableVertexAttribArray(index++);
                ptr += sizeof(float) * 4;
                break;

            default:
                throw std::runtime_error("Type Error");
                break;
            }
        }
    }
    glBindVertexArray(m_VAO);
}

void Mesh_GL::draw(Material& mat)
{
    mat.bind();
    bind();
    switch (m_draw_type)
    {
    case DRAW_ARRAY:
        glDrawArrays(GL_TRIANGLES, 0, m_vertex_buffer.get()->size() * sizeof(float) / m_strike);
        break;

    case DRAW_ELEMENT:
        glDrawElements(GL_TRIANGLES, m_element_buffer.get()->size(), GL_UNSIGNED_INT, 0);
        break;

    default:
        break;
    }
}