#include "core/platform/renderAPI/OpenGL/Mesh_GL.hpp"

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

        int ptr = 0;
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

void Mesh_GL::draw()
{
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

Mesh_GL::Mesh_GL(Shape shape) : Mesh_GL()
{
    switch (shape)
    {
    case Shape::Cube:
    {
         std::vector<float> vertex_data ={
        // |------Position------|  |------Normal-------|  |-Texcoord--|      
            -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    0.0f, 0.0f,
         
            -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    0.0f, 0.0f,
         
            -0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
         
             0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
         
            -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f,
         
            -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    0.0f, 1.0f
        };

        std::vector<unsigned int> index_data = {
            0, 1, 2, 3, 4, 5,
            6, 7, 8, 9, 10, 11,
            12, 13, 14, 15, 16, 17,
            18, 19, 20, 21, 22, 23,
            24, 25, 26, 27, 28, 29,
            30, 31, 32, 33, 34, 35};

        Ref<ArrayBuffer_GL<unsigned int>> EBO = std::make_shared<ArrayBuffer_GL<unsigned int>>();
        Ref<ArrayBuffer_GL<float>> VBO = std::make_shared<ArrayBuffer_GL<float>>();
        EBO->set_data(index_data);
        VBO->set_data(vertex_data);

        set_buffer(VBO, EBO, {ShaderParam_Type::Vec3, ShaderParam_Type::Vec3, ShaderParam_Type::Vec2});
        break;
    }

    case Shape::Quad:{
        std::vector<float> vertex_data ={
        // |------Position------|  |------Normal-------|  |-Texcoord--|      
            -1.0f,  1.0f, 0.0f,    0.0f,  0.0f,  1.0f,    0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f,    0.0f,  0.0f,  1.0f,    0.0f, 0.0f,
             1.0f, -1.0f, 0.0f,    0.0f,  0.0f,  1.0f,    1.0f, 0.0f,
             1.0f,  1.0f, 0.0f,    0.0f,  0.0f,  1.0f,    1.0f, 1.0f,
             1.0f, -1.0f, 0.0f,    0.0f,  0.0f,  1.0f,    1.0f, 0.0f,
            -1.0f,  1.0f, 0.0f,    0.0f,  0.0f,  1.0f,    0.0f, 1.0f 
        };

        std::vector<unsigned int> index_data = {
            0, 1, 2, 3, 4, 5,};

        Ref<ArrayBuffer_GL<unsigned int>> EBO = std::make_shared<ArrayBuffer_GL<unsigned int>>();
        Ref<ArrayBuffer_GL<float>> VBO = std::make_shared<ArrayBuffer_GL<float>>();
        EBO->set_data(index_data);
        VBO->set_data(vertex_data);

        set_buffer(VBO, EBO, {ShaderParam_Type::Vec3, ShaderParam_Type::Vec3, ShaderParam_Type::Vec2});
        break;
    }

    default:
        break;
    }
}
