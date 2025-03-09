// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#pragma once

#include "utils/utils.hpp"
#include "core/platform/renderAPI/ArrayBuffer.hpp"
#include "core/platform/renderAPI/Param.hpp"
#include "core/render/Material.hpp"
#include <glm/glm.hpp>

// Sub-mesh: describes a piece of geometric data
struct SubMesh
{
    unsigned int indexOffset; // The starting position in the index buffer
    unsigned int indexCount;  // The index number of this subgrid
    // Note: Material index is not here, it is managed by Renderer
};

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

    Mesh(Ref<ArrayBuffer<float>> VBO, Ref<ArrayBuffer<unsigned int>> EBO, const std::vector<Param_Type> &layout)
        : m_vertex_buffer(VBO), m_element_buffer(EBO), m_layout(layout), m_binded(false) {}

    void set_buffer(Ref<ArrayBuffer<float>> VBO, Ref<ArrayBuffer<unsigned int>> EBO, const std::vector<Param_Type> &layout);

    void set_buffer(Ref<ArrayBuffer<float>> VBO, const std::vector<Param_Type> &layout);

    static Ref<Mesh> get_base_shape(Shape shape);

    void as_base_shape(Shape shape);

    virtual void draw(Material &mat, glm::mat4 model = glm::mat4(1), int subindex = -1) = 0;

    virtual void bind() = 0;
    virtual ~Mesh() {}

    std::vector<SubMesh> subMeshes;

protected:
    Ref<ArrayBuffer<float>> m_vertex_buffer;
    Ref<ArrayBuffer<unsigned int>> m_element_buffer;
    std::vector<Param_Type> m_layout;
    Draw_Type m_draw_type;
    int m_strike = 0;
    bool m_binded = false;
};

namespace utils
{

    // 顶点数据格式
    struct alignas(4) Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec4 Tangent;
        glm::vec4 Color;
        glm::vec4 TexCoords;
        glm::vec4 BoneWeight;
        glm::vec4 BoneIndex;
        float VertexID;
    };

    inline const std::vector<Param_Type> &get_vertex_layout()
    {
        static std::vector<Param_Type> vertex_layout = {
            Param_Type::Vec3,
            Param_Type::Vec3,
            Param_Type::Vec4,
            Param_Type::Vec4,
            Param_Type::Vec4,
            Param_Type::Vec4,
            Param_Type::Vec4,
            Param_Type::Float,
        };
        return vertex_layout;
    }

    void loadModel(const std::string &path, std::vector<Ref<Mesh>> &meshes);
}