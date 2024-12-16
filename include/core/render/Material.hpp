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
    Material(const std::string &vs, const std::string &fs, bool depth_test = true) : m_depth_test(depth_test)
    {
        auto pipeline = PipelineManager::get(vs, fs);
        m_pipeline = pipeline;
        for (auto i : pipeline->m_shaders)
        {
            m_shader_pms += i->get_params();
        }
    }

    Material(Ref<Pipeline> pipeline)
    {
        m_pipeline = pipeline;
        for (auto i : pipeline->m_shaders)
        {
            m_shader_pms += i->get_params();
        }
    }

    void bind()
    {
        m_pipeline->bind();
        m_pipeline->set_params(m_shader_pms);
    }

    void set_param(const std::string &param, void *ptr)
    {
        m_shader_pms[param].set(ptr);
    }

    void depth_test(bool enable)
    {
        m_depth_test = enable;
    }

    bool get_depth_test()
    {
        return m_depth_test;
    }

    ShaderParamList m_shader_pms;

private:
    Ref<Pipeline> m_pipeline;
    bool m_depth_test;
};