#include "core/platform/renderAPI/Mesh.hpp"

void Mesh::set_buffer(Ref<ArrayBuffer<float>> VBO, Ref<ArrayBuffer<unsigned int>> EBO, const std::vector<Shader::Param_Type> &layout)
{
    m_vertex_buffer = VBO;
    m_element_buffer = EBO;
    m_layout = layout;
    m_binded = false;
    m_draw_type = DRAW_ELEMENT;

    m_strike = 0;

    for (auto i : m_layout)
    {
        switch (i)
        {
        case Shader::Param_Type::Float:
            m_strike += sizeof(float);
            break;
        case Shader::Param_Type::Vec2:
            m_strike += sizeof(float) * 2;
            break;
        case Shader::Param_Type::Vec3:
            m_strike += sizeof(float) * 3;
            break;
        case Shader::Param_Type::Vec4:
            m_strike += sizeof(float) * 4;
            break;

        default:
            break;
        }
    }
}

void Mesh::set_buffer(Ref<ArrayBuffer<float>> VBO, const std::vector<Shader::Param_Type> &layout)
{
    set_buffer(VBO, nullptr, layout);
    m_draw_type = DRAW_ARRAY;
}