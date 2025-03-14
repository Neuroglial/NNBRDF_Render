// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#include "core/render/Material.hpp"
#include "core/platform/renderAPI/RenderAPI.hpp"
#include "scene/PipelineManager.hpp"

Material::Material(const std::string &pipeline_path, bool depth_test, FaceType facetype) : m_depth_test(depth_test), m_face_type(facetype), m_pipeline_path(pipeline_path)
{
    m_pipeline = PipelineManager::get(Root_Path + pipeline_path);
    reloadParamList();
}

void Material::reload()
{
    PipelineManager::reload(Root_Path + m_pipeline_path);
    reloadParamList();
}

void Material::reloadParamList()
{
    m_Params = m_pipeline->get_params();

    for(auto& i:m_Params->m_list)
    {
        std::cout<<ShaderParam_Helper::to_string(i.second->type())<<" "<<i.second->name()<<std::endl;
    }
}

void Material::bind()
{
    RenderAPI::depth_test(m_depth_test);
    RenderAPI::face_culling(m_face_type != Double_Sided, m_face_type == Front);
    m_pipeline->set_params(*m_Params.get());
    m_pipeline->bind();
}

void to_json(nlohmann::json &j, const Ref<Material> &mat)
{
    j["m_pipeline_path"] = mat->m_pipeline_path;
    j["m_depth_test"] = mat->m_depth_test;
    j["m_face_type"] = mat->m_face_type;
}

void from_json(const nlohmann::json& j, Ref<Material>& mat)
{
    mat = std::make_shared<Material>(j["m_pipeline_path"] , j["m_depth_test"], j["m_face_type"]);
}