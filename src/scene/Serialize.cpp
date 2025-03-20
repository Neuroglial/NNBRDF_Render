#include "scene/Serialize.hpp"

#include "core/platform/renderAPI/Param.hpp"
#include "core/platform/renderAPI/RenderAPI.hpp"
#include "scene/ImageManager.hpp"

void to_json(json &j, const Param &p)
{
    j["type"] = p.type();
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
    j["filter"] = p->get_filter();
    j["warp"] = p->get_warp();
    j["channels"] = p->get_channels();
    j["yflip"] = p->get_yflip();
}

void from_json(const json &j, Ref<Texture2D> &p)
{
    std::string path = j["path"].get<std::string>();
    Tex::FilteringMode filter = j["filter"].get<Tex::FilteringMode>();
    Tex::WarppingMode warp = j["warp"].get<Tex::WarppingMode>();
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
    j["filter"] = p->get_filter();
    j["warp"] = p->get_warp();
    j["channels"] = p->get_channels();
    j["hdr"] = p->is_hdr();
}

void from_json(const json &j, Ref<TextureCube> &p)
{
    std::string path = j["path"].get<std::string>();
    Tex::FilteringMode filter = j["filter"].get<Tex::FilteringMode>();
    Tex::WarppingMode warp = j["warp"].get<Tex::WarppingMode>();
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