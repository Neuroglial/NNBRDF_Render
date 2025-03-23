#include "scene/Serialize.hpp"

#include "core/platform/renderAPI/Param.hpp"
#include "core/platform/renderAPI/RenderAPI.hpp"
#include "scene/ImageManager.hpp"
#include "scene/MeshManager.hpp"
#include "scene/MaterialManager.hpp"

void to_json(json &j, const Param &p)
{
    j["type"] = ParamHelper::to_string(p.type());
    j["name"] = p.name();
    j["changed"] = p.changed();

    if (!p.changed())
    {
        j["value"] = "null";
        return;
    }

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

    case ParamType::Texture2D:
    {
        auto value = p.as<PM_Texture2D>();
        if (value->get())
            j["value"] = value->get();
        break;
    }

    case ParamType::TextureCube:
    {
        auto value = p.as<PM_TextureCube>();
        if (value->get())
            j["value"] = value->get();
        break;
    }

    default:
        break;
    }
}

void to_json(json &j, const Ref<Material> &p)
{
    j["shader"] = p->get_pipeline_path();
    j["depth_test"] = p->get_depth_test();
    j["face_type"] = p->get_face_type();

    j["params"] = *p->get_params().get();
}

void from_json(const json &j, Ref<Material> &p)
{
    std::string shader = j["shader"].get<std::string>();
    bool depth_test = j["depth_test"].get<bool>();
    Material::FaceType face_type = j["face_type"].get<Material::FaceType>();

    p = std::make_shared<Material>(shader, depth_test, face_type);
    auto params = j["params"].get<Params>();
    p->m_Params->get(params);
}

void to_json(json &j, const Ref<Texture2D> &p)
{
    j["path"] = p->get_path();
    j["filter"] = Tex::to_string(p->get_filter());
    j["warp"] = Tex::to_string(p->get_warp());
    j["channels"] = p->get_channels();
    j["yflip"] = p->get_yflip();
}

void from_json(const json &j, Ref<Texture2D> &p)
{
    std::string path = j["path"].get<std::string>();
    Tex::FilteringMode filter = Tex::to_filter(j["filter"].get<std::string>());
    Tex::WarppingMode warp = Tex::to_warp(j["warp"].get<std::string>());
    uint32_t channels = j["channels"].get<uint32_t>();
    bool yflip = j["yflip"].get<bool>();

    p = RenderAPI::creator<Texture2D>::crt();

    p->init(warp, filter);
    if (channels & Tex::Bit32)
    {
        p->set_image(ImageManager::get_hdr(path, yflip));
    }
    else
    {
        p->set_image(ImageManager::get(path, yflip));
    }
}

void to_json(json &j, const Ref<TextureCube> &p)
{
    j["path"] = p->get_path();
    j["filter"] = Tex::to_string(p->get_filter());
    j["warp"] = Tex::to_string(p->get_warp());
    j["channels"] = p->get_channels();
    j["hdr"] = p->is_hdr();
}

void from_json(const json &j, Ref<TextureCube> &p)
{
    std::string path = j["path"].get<std::string>();
    Tex::FilteringMode filter = Tex::to_filter(j["filter"].get<std::string>());
    Tex::WarppingMode warp = Tex::to_warp(j["warp"].get<std::string>());
    uint32_t channels = j["channels"].get<uint32_t>();
    bool hrd = j["hdr"].get<bool>();

    p = RenderAPI::creator<TextureCube>::crt();

    p->init(warp, filter);
    if (channels)
    {
        p->set_cubemap(path, hrd);
    }
    else
    {
        p->set_cubemap(path, hrd);
    }
}

void from_json(const json &j, Param *&p)
{
    bool changed = j["changed"].get<bool>();
    if (!changed)
    {
        p = nullptr;
        return;
    }

    ParamType type = ParamHelper::to_type(j["type"].get<std::string>());

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

    case ParamType::Texture2D:
    {
        p = new PM_Texture2D(j["name"].get<std::string>(), j["value"].get<Ref<Texture2D>>());
        break;
    }

    case ParamType::TextureCube:
    {
        p = new PM_TextureCube(j["name"].get<std::string>(), j["value"].get<Ref<TextureCube>>());
        break;
    }

    default:
        break;
    }

    p->set_changed(j["changed"].get<bool>());
}

void to_json(json &j, const MeshComponent &p)
{
    j["cast_shadow"] = p.m_castShadow;

    if (p.m_mesh)
        j["mesh"] = p.m_mesh->get_path();
    else
        j["mesh"] = "null";
}

void from_json(const json &j, MeshComponent &p)
{
    from_json_ptr(j, &p);
}

void from_json_ptr(const json &j, MeshComponent *p)
{
    p->m_castShadow = j["cast_shadow"].get<bool>();
    std::string path = j["mesh"].get<std::string>();

    if (path != "null")
        p->m_mesh = MeshManager::get(path);
}

void to_json(json &j, const TransformComponent &p)
{
    j["position"] = p.m_position;
    j["scale"] = p.m_scale;
    j["rotation"] = p.get_rotEuler();
    j["static"] = p.m_static;
}

void from_json(const json &j, TransformComponent &p)
{
    from_json_ptr(j, &p);
}

void from_json_ptr(const json &j, TransformComponent *p)
{
    p->m_position = j["position"].get<glm::vec3>();
    p->m_scale = j["scale"].get<glm::vec3>();
    p->set_rotEuler(j["rotation"].get<glm::vec3>());
    p->m_static = j["static"].get<bool>();
}

void to_json(json &j, const RendererComponent &p)
{
    auto& j_mats = j["materials"];
    j_mats = json::array();
    for (int i = 0; i < p.m_materials.size(); ++i)
    {
        j_mats.push_back(p.m_materials[i]->get_path());
    }
}

void from_json(const json &j, RendererComponent &p)
{
}

void from_json_ptr(const json &j, RendererComponent *p)
{
    auto j_mats = j["materials"];
    for (int i = 0; i < j_mats.size(); ++i)
    {
        auto path = j_mats[i].get<std::string>();
        p->m_materials.push_back(MaterialManager::get(path));
    }
}