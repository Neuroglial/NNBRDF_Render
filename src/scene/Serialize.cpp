#include "scene/Serialize.hpp"

#include "core/platform/renderAPI/Param.hpp"

void to_json(json &j, const SD_Param &p)
{
    switch (p.m_type)
    {
    case Param_Type::Int:
        j = AS(int, p.m_value_ptr);
        break;

    case Param_Type::Float:
        j = AS(float, p.m_value_ptr);
        break;

    case Param_Type::Vec2:
        j = AS(glm::vec2, p.m_value_ptr);
        break;

    case Param_Type::Vec3:
        j = AS(glm::vec3, p.m_value_ptr);
        break;

    case Param_Type::Vec4:
        j = AS(glm::vec4, p.m_value_ptr);
        break;

    case Param_Type::Mat2:
        j = AS(glm::mat2, p.m_value_ptr);
        break;

    case Param_Type::Mat3:
        j = AS(glm::mat3, p.m_value_ptr);
        break;

    case Param_Type::Mat4:
        j = AS(glm::mat4, p.m_value_ptr);
        break;

    default:
        break;
    }
}

void from_json(const json &j, SD_Param &p)
{
}