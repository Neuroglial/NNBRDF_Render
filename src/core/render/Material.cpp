// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#include "core/render/Material.hpp"
#include "core/platform/renderAPI/RenderAPI.hpp"
#include "scene/PipelineManager.hpp"

Material::Material(const std::string &pipeline_path, bool depth_test, FaceType facetype) : m_depth_test(depth_test), m_face_type(facetype), m_pipeline_path(pipeline_path)
{
    m_pipeline = PipelineManager::get(pipeline_path);
    m_shader_pms = m_pipeline->get_params_list();
}

void Material::reload()
{
    PipelineManager::reload(m_pipeline_path);
    reloadParamList();
}

void Material::reloadParamList()
{
    m_shader_pms = m_pipeline->get_params_list();
}

void Material::bind()
{
    RenderAPI::depth_test(m_depth_test);
    RenderAPI::face_culling(m_face_type != Double_Sided, m_face_type == Front);
    m_pipeline->bind();
    m_pipeline->set_params(*m_shader_pms);
}