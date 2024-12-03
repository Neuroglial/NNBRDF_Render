#pragma once

#include "utils/utils.hpp"
#include "core/platform/renderAPI/ArrayBuffer.hpp"
#include "core/platform/renderAPI/ShaderParam.hpp"

class Mesh{
    public:
    Mesh():
        m_vertex_buffer(nullptr),m_element_buffer(nullptr),m_binded(false){}
    Mesh(Ref<ArrayBuffer<float>> VBO,Ref<ArrayBuffer<unsigned int>> EBO,const std::vector<Shader::Param_Type>& layout):
        m_vertex_buffer(VBO),m_element_buffer(EBO),m_layout(layout),m_binded(false){}

    void set_buffer(Ref<ArrayBuffer<float>> VBO,Ref<ArrayBuffer<unsigned int>> EBO,const std::vector<Shader::Param_Type>& layout){
        m_vertex_buffer = VBO;
        m_element_buffer = EBO;
        m_layout = layout;
        m_binded = false;
    }

    virtual void bind() = 0;

    protected:
    Ref<ArrayBuffer<float>> m_vertex_buffer;
    Ref<ArrayBuffer<unsigned int>> m_element_buffer;
    std::vector<Shader::Param_Type> m_layout;
    bool m_binded;
};