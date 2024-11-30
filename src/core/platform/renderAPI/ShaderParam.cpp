#include "core/platform/renderAPI/ShaderParam.hpp"

void print_params(Shader::ParamListBase &plb)
{
    for (auto &param : plb.m_param_list)
    {
        switch (param.second.m_type)
        {
        case Shader::Parameter_Type::Float:
            std::cout << "float " + param.first + " = " + std::to_string(*(float *)param.second.m_value_ptr) << std::endl;
            break;
        case Shader::Parameter_Type::Vec2:
            std::cout << "vec2 " + param.first + " = " + std::to_string(*(glm::vec2 *)param.second.m_value_ptr) << std::endl;
            break;
        case Shader::Parameter_Type::Vec3:
            std::cout << "vec3 " + param.first + " = " + std::to_string(*(glm::vec3 *)param.second.m_value_ptr) << std::endl;
            break;
        case Shader::Parameter_Type::Vec4:
            std::cout << "vec4 " + param.first + " = " + std::to_string(*(glm::vec4 *)param.second.m_value_ptr) << std::endl;
            break;

        default:
            break;
        }
    }
}