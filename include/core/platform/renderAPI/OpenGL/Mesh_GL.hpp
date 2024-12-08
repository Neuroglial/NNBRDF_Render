#pragma once
#include "core/platform/renderAPI/Mesh.hpp"
#include "core/platform/renderAPI/OpenGL/ArrayBuffer_GL.hpp"
#include "core/render/Material.hpp"

class Mesh_GL : public Mesh
{
public:
    Mesh_GL() : Mesh(), m_VAO(0) {};

    Mesh_GL(Shape shape);

    Mesh_GL(Ref<ArrayBuffer<float>> VBO, Ref<ArrayBuffer<unsigned int>> EBO, const std::vector<ShaderParam_Type> &layout) : Mesh(VBO, EBO, layout), m_VAO(0) {}

    void bind();

    virtual void draw(Material& mat) override;

    virtual ~Mesh_GL()
    {
        if (m_VAO)
        {
            glDeleteVertexArrays(1, &m_VAO);
        }
    }

private:
    unsigned int m_VAO = 0;
};