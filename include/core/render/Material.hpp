#pragma once
#include <vector>
#include <string>
#include "core/platform/renderAPI/Shader.hpp"
#include "scene/PipelineManager.hpp"

class Material
{
public:
    Material(const std::string &vs, const std::string &fs)
    {
        auto pipeline = PipelineManager::get(vs,fs);
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

    void set_param(const std::string& param,void* ptr)
    {
        m_shader_pms[param].set(ptr);
    }

    ShaderParamList m_shader_pms;

private:
    Ref<Pipeline> m_pipeline;
};