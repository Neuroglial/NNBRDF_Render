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

    enum Shape
    {
        Cube,
        Quad
    };

    Mesh() : m_vertex_buffer(nullptr), m_element_buffer(nullptr), m_binded(false) {}

    Mesh(Ref<ArrayBuffer<float>> VBO, Ref<ArrayBuffer<unsigned int>> EBO, const std::vector<ShaderParam_Type> &layout) : m_vertex_buffer(VBO), m_element_buffer(EBO), m_layout(layout), m_binded(false) {}

    void set_buffer(Ref<ArrayBuffer<float>> VBO, Ref<ArrayBuffer<unsigned int>> EBO, const std::vector<ShaderParam_Type> &layout);

    void set_buffer(Ref<ArrayBuffer<float>> VBO, const std::vector<ShaderParam_Type> &layout);

    virtual void bind() = 0;
    virtual void draw() = 0;

    virtual ~Mesh(){}

protected:
    Ref<ArrayBuffer<float>> m_vertex_buffer;
    Ref<ArrayBuffer<unsigned int>> m_element_buffer;
    std::vector<ShaderParam_Type> m_layout;
    Draw_Type m_draw_type;
    int m_strike;
    bool m_binded;
};