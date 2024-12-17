// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#pragma once

#include "utils/utils.hpp"
#include "core/platform/renderAPI/ArrayBuffer.hpp"
#include "core/platform/renderAPI/ShaderParam.hpp"
#include "core/render/Material.hpp"

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
        None,
        Cube,
        Quad
    };

    Mesh() : m_vertex_buffer(nullptr), m_element_buffer(nullptr), m_binded(false) {}

    Mesh(Ref<ArrayBuffer<float>> VBO, Ref<ArrayBuffer<unsigned int>> EBO, const std::vector<ShaderParam_Type> &layout)
        : m_vertex_buffer(VBO), m_element_buffer(EBO), m_layout(layout), m_binded(false) {}

    void set_buffer(Ref<ArrayBuffer<float>> VBO, Ref<ArrayBuffer<unsigned int>> EBO, const std::vector<ShaderParam_Type> &layout);

    void set_buffer(Ref<ArrayBuffer<float>> VBO, const std::vector<ShaderParam_Type> &layout);

    void as_base_shape(Shape shape);

    virtual void draw(Material &mat) = 0;
    virtual ~Mesh() {}

protected:

    
    Ref<ArrayBuffer<float>> m_vertex_buffer;
    Ref<ArrayBuffer<unsigned int>> m_element_buffer;
    std::vector<ShaderParam_Type> m_layout;
    Draw_Type m_draw_type;
    int m_strike = 0;
    bool m_binded = false;
};

namespace utils
{
    void loadModel(const std::string &path, std::vector<Ref<Mesh>> &meshes);
}