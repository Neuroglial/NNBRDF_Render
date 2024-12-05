#pragma once
#include "core/platform/renderAPI/Mesh.hpp"
#include "core/platform/renderAPI/OpenGL/ArrayBuffer_GL.hpp"

class Mesh_GL:public Mesh{
    public:
    Mesh_GL():Mesh(),m_vertex_array(0){};

    Mesh_GL(Shape shape);

    Mesh_GL(Ref<ArrayBuffer<float>> VBO,Ref<ArrayBuffer<unsigned int>> EBO,const std::vector<Shader::Param_Type>& layout):
        Mesh(VBO,EBO,layout),m_vertex_array(0){}

    virtual void bind() override;

    virtual void draw() override;

    private:
    unsigned int m_vertex_array;
};