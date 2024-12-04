#pragma once

#include "utils/utils.hpp"
#include "core/platform/renderAPI/ArrayBuffer.hpp"
#include "core/platform/renderAPI/ShaderParam.hpp"

class Mesh
{
public:
    enum Draw_Type
    {
        DRAW_ARRAY,
        DRAW_ELEMENT,
    };

    Mesh() : m_vertex_buffer(nullptr), m_element_buffer(nullptr), m_binded(false) {}
    Mesh(Ref<ArrayBuffer<float>> VBO, Ref<ArrayBuffer<unsigned int>> EBO, const std::vector<Shader::Param_Type> &layout) : m_vertex_buffer(VBO), m_element_buffer(EBO), m_layout(layout), m_binded(false) {}

    void set_buffer(Ref<ArrayBuffer<float>> VBO, Ref<ArrayBuffer<unsigned int>> EBO, const std::vector<Shader::Param_Type> &layout)
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

    void set_buffer(Ref<ArrayBuffer<float>> VBO, const std::vector<Shader::Param_Type> &layout)
    {
        set_buffer(VBO,nullptr,layout);
        m_draw_type = DRAW_ARRAY;
    }

    virtual void bind() = 0;
    virtual void draw() = 0;

protected:
    Ref<ArrayBuffer<float>> m_vertex_buffer;
    Ref<ArrayBuffer<unsigned int>> m_element_buffer;
    std::vector<Shader::Param_Type> m_layout;
    Draw_Type m_draw_type;
    int m_strike;
    bool m_binded;
};