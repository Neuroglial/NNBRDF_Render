#pragma once

#include "scene/Component.hpp"
#include "nlohmann/json.hpp"
#include "glm/glm.hpp"
#include "fstream"

#include "scene/GameObject.hpp"

// void to_json(json &j, const SD_ParamList &paramList)
// {
// }

// void from_json(const json &j, SD_ParamList &paramList)
// {
// }

struct SD_Param;

using namespace nlohmann;

namespace glm
{
    inline void to_json(json &j, const vec2 &p)
    {
        float v[] = {p[0], p[1]};
        j = v;
    }

    inline void from_json(const json &j, vec2 &p)
    {
        p[0] = j[0].get<float>();
        p[1] = j[1].get<float>();
    }

    inline void to_json(json &j, const vec3 &p)
    {
        float v[] = {p[0], p[1], p[2]};
        j = v;
    }

    inline void from_json(const json &j, vec3 &p)
    {
        p[0] = j[0].get<float>();
        p[1] = j[1].get<float>();
        p[2] = j[2].get<float>();
    }

    inline void to_json(json &j, const vec4 &p)
    {
        float v[] = {p[0], p[1], p[2], p[3]};
        j = v;
    }

    inline void from_json(const json &j, vec4 &p)
    {
        p[0] = j[0].get<float>();
        p[1] = j[1].get<float>();
        p[2] = j[2].get<float>();
        p[3] = j[3].get<float>();
    }

    inline void to_json(json &j, const mat2 &p)
    {
        j[0] = (vec2)p[0];
        j[1] = (vec2)p[1];
    }

    inline void from_json(const json &j, mat2 &p)
    {
        p[0] = j[0].get<vec2>();
        p[1] = j[1].get<vec2>();
    }

    inline void to_json(json &j, const mat3 &p)
    {
        j[0] = (vec3)p[0];
        j[1] = (vec3)p[1];
        j[2] = (vec3)p[2];
    }

    inline void from_json(const json &j, mat3 &p)
    {
        p[0] = j[0].get<vec3>();
        p[1] = j[1].get<vec3>();
        p[2] = j[2].get<vec3>();
    }

    inline void to_json(json &j, const mat4 &p)
    {
        j[0] = (vec4)p[0];
        j[1] = (vec4)p[1];
        j[2] = (vec4)p[2];
        j[3] = (vec4)p[3];
    }

    inline void from_json(const json &j, mat4 &p)
    {
        p[0] = j[0].get<vec4>();
        p[1] = j[1].get<vec4>();
        p[2] = j[2].get<vec4>();
        p[3] = j[3].get<vec4>();
    }
};

void to_json(json &j, const Ref<Texture2D> &p);
void from_json(const json &j, Ref<Texture2D> &p);

void to_json(json &j, const Ref<Material> &p);
void from_json(const json &j, Ref<Material> &p);

void to_json(json &j, const Ref<TextureCube> &p);
void from_json(const json &j, Ref<TextureCube> &p);

void to_json(json &j, const Param &p);
void from_json(const json &j, Param *&p);

void to_json(json &j, const MeshComponent &p);
void from_json(const json &j, MeshComponent &p);
void from_json_ptr(const json &j, MeshComponent *p);

void to_json(json &j, const TransformComponent &p);
void from_json(const json &j, TransformComponent &p);
void from_json_ptr(const json &j, TransformComponent *p);

void to_json(json &j, const RendererComponent &p);
void from_json(const json &j, RendererComponent &p);
void from_json_ptr(const json &j, RendererComponent *p);

void to_json(json &j, const PointLightComponent &p);
void from_json(const json &j, PointLightComponent &p);
void from_json_ptr(const json &j, PointLightComponent *p);

void to_json(json &j, const CameraComponet &p);
void from_json(const json &j, CameraComponet &p);
void from_json_ptr(const json &j, CameraComponet *p);

void to_json(json &j, const ScriptComponent &p);

void to_json(json &j, const Ref<GameObject> &p);
void from_json(const json &j, Ref<GameObject> &p);
void from_json_ptr(const json &j, Ref<GameObject> &p);

inline void to_json(json &j, const Params &p)
{
    j = json::array();
    for (auto &i : p.m_list)
    {
        j.push_back(*i.second);
    }
}

inline void from_json(const json &j, Params &p)
{
    for (int i = 0; i < j.size(); ++i)
    {
        auto *pm = j[i].get<Param *>();
        if (pm)
            p.add(Ref<Param>(pm));
    }
}

inline json from_file_json(const std::string &path)
{
    std::ifstream i(Root_Path + path);
    json j;
    i >> j;

    return j;
}

template <typename T>
T from_file(const std::string &path)
{
    std::ifstream i(Root_Path + path);
    json j;
    i >> j;

    T ret = j;
    return ret;
}

template <typename T>
void to_file(const T &Object, const std::string &path)
{
    std::ofstream o(Root_Path + path, std::ios::out);
    if (o.is_open())
    {
        json j = Object;
        o << std::setw(4) << j << std::endl;
        o.close();
    }
}