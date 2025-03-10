// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#include "core/render/Material.hpp"
#include "core/platform/renderAPI/RenderAPI.hpp"
#include "scene/PipelineManager.hpp"

Material::Material(const std::string &pipeline_path, bool depth_test, FaceType facetype) : m_depth_test(depth_test), m_face_type(facetype), m_pipeline_path(pipeline_path)
{
    m_pipeline = PipelineManager::get(Root_Path + pipeline_path);
    m_shader_pms = std::make_shared<SD_ParamList>(*m_pipeline->get_params_list().get());
}

void Material::reload()
{
    PipelineManager::reload(Root_Path + m_pipeline_path);
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

void to_json(nlohmann::json &j, const Ref<Material> &mat)
{
    j["m_pipeline_path"] = mat->m_pipeline_path;
    j["m_depth_test"] = mat->m_depth_test;
    j["m_face_type"] = mat->m_face_type;

    auto params = mat->get_params_list();
}

void from_json(const nlohmann::json& j, Ref<Material>& mat)
{
    mat = std::make_shared<Material>(j["m_pipeline_path"] , j["m_depth_test"], j["m_face_type"]);
}