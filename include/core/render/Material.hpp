// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#pragma once
#include <vector>
#include <string>
#include "core/platform/renderAPI/Shader.hpp"
#include "scene/PipelineManager.hpp"

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

    Material(const std::string &vs, const std::string &fs, bool depth_test, FaceType facetype = Front);

    Material(const std::string &vs, const std::string &gs, const std::string &fs, bool depth_test, FaceType facetype = Front);

    Material(Ref<Pipeline> pipeline);

    void bind();

    inline Ref<ShaderParamList> get_params_list()
    {
        return m_shader_pms;
    }

    inline void set_param(const std::string &param, void *ptr)
    {
        (*m_shader_pms)[param].set(ptr);
    }

    inline void depth_test(bool enable)
    {
        m_depth_test = enable;
    }

    inline bool get_depth_test()
    {
        return m_depth_test;
    }

    Ref<ShaderParamList> m_shader_pms;

private:
    Ref<Pipeline> m_pipeline;
    bool m_depth_test;
    FaceType m_face_type;
};