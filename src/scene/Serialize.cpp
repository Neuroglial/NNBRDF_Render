#include "scene/Serialize.hpp"

#include "core/platform/renderAPI/Param.hpp"

void to_json(json &j, const Param &p)
{
    j["type"] = p.type();
    j["name"] = p.name();
    switch (p.type())
    {

    case ParamType::Int:
    {
        auto value = p.as<PM_Int>();
        j["value"] = value->get();
        break;
    }

    case ParamType::Float:
    {
        auto value = p.as<PM_Float>();
        j["value"] = value->get();
        break;
    }

    case ParamType::Vec2:
    {
        auto value = p.as<PM_Vec2>();
        j["value"] = value->get();
        break;
    }

    case ParamType::Vec3:
    {
        auto value = p.as<PM_Vec3>();
        j["value"] = value->get();
        break;
    }

    case ParamType::Vec4:
    {
        auto value = p.as<PM_Vec4>();
        j["value"] = value->get();
        break;
    }

    case ParamType::Mat2:
    {
        auto value = p.as<PM_Mat2>();
        j["value"] = value->get();
        break;
    }

    case ParamType::Mat3:
    {
        auto value = p.as<PM_Mat3>();
        j["value"] = value->get();
        break;
    }

    case ParamType::Mat4:
    {
        auto value = p.as<PM_Mat4>();
        j["value"] = value->get();
        break;
    }

    default:
        break;
    }
}

void from_json(const json &j, Param *&p)
{
    ParamType type = j["type"].get<ParamType>();

    switch (type)
    {

    case ParamType::Int:
    {
        p = new PM_Int(j["name"].get<std::string>(), j["value"].get<int>());
        break;
    }

    case ParamType::Float:
    {
        p = new PM_Float(j["name"].get<std::string>(), j["value"].get<float>());
        break;
    }

    case ParamType::Vec2:
    {
        p = new PM_Vec2(j["name"].get<std::string>(), j["value"].get<glm::vec2>());
        break;
    }

    case ParamType::Vec3:
    {
        p = new PM_Vec3(j["name"].get<std::string>(), j["value"].get<glm::vec3>());
        break;
    }

    case ParamType::Vec4:
    {
        p = new PM_Vec4(j["name"].get<std::string>(), j["value"].get<glm::vec4>());
        break;
    }

    case ParamType::Mat2:
    {
        p = new PM_Mat2(j["name"].get<std::string>(), j["value"].get<glm::mat2>());
        break;
    }

    case ParamType::Mat3:
    {
        p = new PM_Mat3(j["name"].get<std::string>(), j["value"].get<glm::mat3>());
        break;
    }

    case ParamType::Mat4:
    {
        p = new PM_Mat4(j["name"].get<std::string>(), j["value"].get<glm::mat4>());
        break;
    }

    default:
        break;
    }
}