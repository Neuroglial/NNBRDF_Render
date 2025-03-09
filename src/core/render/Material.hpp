// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#pragma once
#include <vector>
#include <string>
#include "core/platform/renderAPI/Shader.hpp"
#include "scene/PipelineManager.hpp"

#include "nlohmann/json.hpp"

class Material
{
public:
    enum FaceType
    {
        Front,
        Back,
        Double_Sided,
    };

    Material(const std::string &pipeline_path, bool depth_test, FaceType facetype = Front);

    void reload();

    void reloadParamList();

    void bind();

    std::string get_shaderName()
    {
        auto end1 = m_pipeline_path.find_last_of('/');
        auto end2 = m_pipeline_path.find_last_of('\\');

        if (end2 != std::string::npos)
        {
            if (end1 != std::string::npos)
            {
                end1 = std::max(end1, end2);
            }
            else
                end1 = end2;
        }

        return m_pipeline_path.substr(end1 + 1, m_pipeline_path.find_last_of('.') - end1 - 1);
    }

    inline Ref<SD_ParamList> get_params_list()
    {
        return m_shader_pms;
    }

    inline void set_param(const std::string &param, void *ptr)
    {
        if (auto m_ptr = (*m_shader_pms)[param])
        {
            m_ptr->set(ptr);
        }
    }

    inline void set_paramList(const std::string &param, int index, void *ptr)
    {
        set_param(param + "[" + std::to_string(index) + "]", ptr);
    }

    inline void depth_test(bool enable)
    {
        m_depth_test = enable;
    }

    inline bool get_depth_test()
    {
        return m_depth_test;
    }

    std::string &get_pipeline_path()
    {
        return m_pipeline_path;
    }

    Ref<SD_ParamList> m_shader_pms;

private:
    Ref<Pipeline> m_pipeline;
    std::string m_pipeline_path;
    bool m_depth_test;
    FaceType m_face_type;
    inline static bool m_reload = false;


    friend void to_json(nlohmann::json &j, const Ref<Material> &mat);
};


void to_json(nlohmann::json &j, const Ref<Material> &mat);

void from_json(const nlohmann::json& j, Ref<Material>& mat);