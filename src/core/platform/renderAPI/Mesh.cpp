#include "core/platform/renderAPI/Mesh.hpp"
#include "core/platform/renderAPI/RenderAPI.hpp"

void Mesh::as_base_shape(Shape shape)
{
    switch (shape)
    {
    case Shape::Cube:
    {
         std::vector<float> vertex_data ={
        // |------Position------|  |------Normal-------|  |-Texcoord--|      
            -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    0.0f, 0.0f,
             0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
             0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    1.0f, 1.0f,
            -0.5f,  0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,    0.0f,  0.0f, -1.0f,    0.0f, 0.0f,
         
            -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    0.0f, 0.0f,
             0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    1.0f, 1.0f,
            -0.5f,  0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,    0.0f,  0.0f,  1.0f,    0.0f, 0.0f,
         
            -0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
            -0.5f, -0.5f, -0.5f,   -1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
            -0.5f, -0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,   -1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
         
             0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
             0.5f,  0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 1.0f,
             0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 1.0f,
             0.5f, -0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    0.0f, 0.0f,
             0.5f,  0.5f,  0.5f,    1.0f,  0.0f,  0.0f,    1.0f, 0.0f,
         
            -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f,
             0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 1.0f,
             0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
             0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    1.0f, 0.0f,
            -0.5f, -0.5f,  0.5f,    0.0f, -1.0f,  0.0f,    0.0f, 0.0f,
            -0.5f, -0.5f, -0.5f,    0.0f, -1.0f,  0.0f,    0.0f, 1.0f,
         
            -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    0.0f, 1.0f,
             0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 1.0f,
             0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
             0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    1.0f, 0.0f,
            -0.5f,  0.5f,  0.5f,    0.0f,  1.0f,  0.0f,    0.0f, 0.0f,
            -0.5f,  0.5f, -0.5f,    0.0f,  1.0f,  0.0f,    0.0f, 1.0f
        };

        std::vector<unsigned int> index_data = {
             0,  1,  2,  3,  4,  5,
             6,  7,  8,  9, 10, 11,
            12, 13, 14, 15, 16, 17,
            18, 19, 20, 21, 22, 23,
            24, 25, 26, 27, 28, 29,
            30, 31, 32, 33, 34, 35};

        auto EBO = RenderAPI::creator<ArrayBuffer<unsigned int>>::crt();
        Ref<ArrayBuffer<float>> VBO(RenderAPI::creator<ArrayBuffer<float>>::crt());

        EBO->set_data(index_data);
        VBO->set_data(vertex_data);

        set_buffer(VBO, EBO, {ShaderParam_Type::Vec3, ShaderParam_Type::Vec3, ShaderParam_Type::Vec2});
        break;
    }

    case Shape::Quad:{
        std::vector<float> vertex_data ={
        // |------Position------|  |------Normal-------|  |-Texcoord--|      
            -1.0f,  1.0f, 0.0f,    0.0f,  0.0f,  1.0f,    0.0f, 1.0f,
            -1.0f, -1.0f, 0.0f,    0.0f,  0.0f,  1.0f,    0.0f, 0.0f,
             1.0f, -1.0f, 0.0f,    0.0f,  0.0f,  1.0f,    1.0f, 0.0f,
             1.0f,  1.0f, 0.0f,    0.0f,  0.0f,  1.0f,    1.0f, 1.0f,
             1.0f, -1.0f, 0.0f,    0.0f,  0.0f,  1.0f,    1.0f, 0.0f,
            -1.0f,  1.0f, 0.0f,    0.0f,  0.0f,  1.0f,    0.0f, 1.0f 
        };

        std::vector<unsigned int> index_data = {
            0, 1, 2, 3, 4, 5,};

        Ref<ArrayBuffer<unsigned int>> EBO(RenderAPI::creator<ArrayBuffer<unsigned int>>::crt());
        Ref<ArrayBuffer<float>> VBO(RenderAPI::creator<ArrayBuffer<float>>::crt());

        EBO->set_data(index_data);
        VBO->set_data(vertex_data);

        set_buffer(VBO, EBO, {ShaderParam_Type::Vec3, ShaderParam_Type::Vec3, ShaderParam_Type::Vec2});
        break;
    }

    default:
        break;
    }
}

void Mesh::set_buffer(Ref<ArrayBuffer<float>> VBO, Ref<ArrayBuffer<unsigned int>> EBO, const std::vector<ShaderParam_Type> &layout)
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
        case ShaderParam_Type::Float:
            m_strike += sizeof(float);
            break;
        case ShaderParam_Type::Vec2:
            m_strike += sizeof(float) * 2;
            break;
        case ShaderParam_Type::Vec3:
            m_strike += sizeof(float) * 3;
            break;
        case ShaderParam_Type::Vec4:
            m_strike += sizeof(float) * 4;
            break;

        default:
            break;
        }
    }
}

void Mesh::set_buffer(Ref<ArrayBuffer<float>> VBO, const std::vector<ShaderParam_Type> &layout)
{
    set_buffer(VBO, nullptr, layout);
    m_draw_type = DRAW_ARRAY;
}