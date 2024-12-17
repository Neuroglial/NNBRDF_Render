// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#include "core/render/Material.hpp"
#include "core/platform/renderAPI/RenderAPI.hpp"

Material::Material(const std::string &vs, const std::string &fs, bool depth_test, FaceType facetype) : m_depth_test(depth_test), m_face_type(facetype)
{
    auto pipeline = PipelineManager::get(vs, fs);
    m_pipeline = pipeline;
    for (auto i : pipeline->m_shaders)
    {
        m_shader_pms += i->get_params();
    }
}

Material::Material(const std::string &vs, const std::string &gs, const std::string &fs, bool depth_test, FaceType facetype) : m_depth_test(depth_test), m_face_type(facetype)
{
    auto pipeline = PipelineManager::get(vs, gs, fs);
    m_pipeline = pipeline;
    for (auto i : pipeline->m_shaders)
    {
        m_shader_pms += i->get_params();
    }
}

Material::Material(Ref<Pipeline> pipeline)
{
    m_pipeline = pipeline;
    for (auto i : pipeline->m_shaders)
    {
        m_shader_pms += i->get_params();
    }
}

void Material::bind()
{
    RenderAPI::depth_test(m_depth_test);
    RenderAPI::face_culling(m_face_type != Double_Sided, m_face_type == Front);
    m_pipeline->bind();
    m_pipeline->set_params(m_shader_pms);
}