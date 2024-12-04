#pragma once
#include "core/platform/renderAPI/Mesh.hpp"
#include "core/platform/renderAPI/OpenGL/ArrayBuffer_GL.hpp"

class Mesh_GL:public Mesh{
    public:
    Mesh_GL():Mesh(),m_vertex_array(0){};
    Mesh_GL(Ref<ArrayBuffer<float>> VBO,Ref<ArrayBuffer<unsigned int>> EBO,const std::vector<Shader::Param_Type>& layout):
        Mesh(VBO,EBO,layout),m_vertex_array(0){}

    virtual void bind() override{
        if(!m_binded){
            if(m_vertex_array)
                glDeleteVertexArrays(1, &m_vertex_array);

            glGenVertexArrays(1, &m_vertex_array);
            glBindVertexArray(m_vertex_array);

            auto VBO = static_cast<ArrayBuffer_GL<float>*>(m_vertex_buffer.get());
            auto EBO = static_cast<ArrayBuffer_GL<unsigned int>*>(m_element_buffer.get());

            glBindBuffer(GL_ARRAY_BUFFER,VBO->get_id());
            if(EBO)
                glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO->get_id());

            int ptr = 0;
            int index = 0;

            for(auto i:m_layout){
                switch (i)
                {
                case Shader::Param_Type::Float:
                    glVertexAttribPointer(index, 1, GL_FLOAT, GL_FALSE,  m_strike, (void*)ptr);
                    glEnableVertexAttribArray(index++);
                    ptr+=sizeof(float);
                    break;
                case Shader::Param_Type::Vec2:
                    glVertexAttribPointer(index, 2, GL_FLOAT, GL_FALSE,  m_strike, (void*)ptr);
                    glEnableVertexAttribArray(index++);
                    ptr+=sizeof(float)*2;
                    break;
                case Shader::Param_Type::Vec3:
                    glVertexAttribPointer(index, 3, GL_FLOAT, GL_FALSE,  m_strike, (void*)ptr);
                    glEnableVertexAttribArray(index++);
                    ptr+=sizeof(float)*3;
                    break;
                case Shader::Param_Type::Vec4:
                    glVertexAttribPointer(index, 4, GL_FLOAT, GL_FALSE,  m_strike, (void*)ptr);
                    glEnableVertexAttribArray(index++);
                    ptr+=sizeof(float)*4;
                    break;
                
                default:
                    throw std::runtime_error("Type Error");
                    break;
                }
            }
        }
        glBindVertexArray(m_vertex_array);
    }

    virtual void draw() override{
        bind();
        switch (m_draw_type)
        {
        case DRAW_ARRAY:
            glDrawArrays(GL_TRIANGLES, 0, m_vertex_buffer.get()->size()*sizeof(float)/m_strike);
            break;

        case DRAW_ELEMENT:
            glDrawElements(GL_TRIANGLES, m_element_buffer.get()->size(), GL_UNSIGNED_INT, 0);
            break;
        
        default:
            break;
        }
    }

    private:
    unsigned int m_vertex_array;
};