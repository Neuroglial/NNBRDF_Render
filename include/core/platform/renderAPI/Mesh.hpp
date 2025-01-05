// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#pragma once

#include "utils/utils.hpp"
#include "core/platform/renderAPI/ArrayBuffer.hpp"
#include "core/platform/renderAPI/ShaderParam.hpp"
#include "core/render/Material.hpp"
#include <glm/glm.hpp>

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

    static Ref<Mesh> get_base_shape(Shape shape);

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
    struct alignas(4) Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec4 Tangent;
        glm::vec4 Color;
        glm::vec4 TexCoord;
        glm::vec4 BoneWeight;
        glm::vec4 BoneIndex;
        float VertexID;
    };

    inline const std::vector<ShaderParam_Type> &get_vertex_layout()
    {
        static std::vector<ShaderParam_Type> vertex_layout = {
            ShaderParam_Type::Vec3,
            ShaderParam_Type::Vec3,
            ShaderParam_Type::Vec4,
            ShaderParam_Type::Vec4,
            ShaderParam_Type::Vec4,
            ShaderParam_Type::Vec4,
            ShaderParam_Type::Vec4,
            ShaderParam_Type::Float,
            };
        return vertex_layout;
    }

    void loadModel(const std::string &path, std::vector<Ref<Mesh>> &meshes);
}