// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#include "scene/PipelineManager.hpp"
#include "core/platform/renderAPI/RenderAPI.hpp"
#include "scene/ShaderManager.hpp"

Ref<Pipeline> PipelineManager::get(const std::string &pipeline_path)
{
    auto mp = get_map();
    auto i = mp->find(pipeline_path);
    if (i != mp->end())
        return i->second;

    auto pipe = RenderAPI::creator<Pipeline>::crt();

    std::string code = utils::read_from_file_with_include(pipeline_path);

    auto vertexCode = utils::get_between(code, "#vertex", "#end");
    Assert(vertexCode.size());
    auto vs = RenderAPI::creator<Shader>::crt();
    vs->set(vertexCode[0], Shader_Type::VERTEX_SHADER);
    pipe->attach_shader(vs);

    auto geometryCode = utils::get_between(code, "#geometry", "#end");
    if (geometryCode.size())
    {
        auto gs = RenderAPI::creator<Shader>::crt();
        gs->set(geometryCode[0], Shader_Type::GEOMETRY_SHADER);
        pipe->attach_shader(gs);
    }

    auto fragmentCode = utils::get_between(code, "#fragment", "#end");
    Assert(fragmentCode.size());
    auto fs = RenderAPI::creator<Shader>::crt();
    fs->set(fragmentCode[0], Shader_Type::FRAGMENT_SHADER);
    pipe->attach_shader(fs);

    mp->emplace(pipeline_path, pipe);
    return pipe;
}

Ref<Pipeline> PipelineManager::reload(const std::string &pipeline_path)
{
    auto mp = get_map();
    auto i = mp->find(pipeline_path);

    Assert(i != mp->end());

    auto &pipe = i->second;

    pipe->clear();

    std::string code = utils::read_from_file_with_include(pipeline_path);

    auto vertexCode = utils::get_between(code, "#vertex", "#end");
    Assert(vertexCode.size());
    auto vs = RenderAPI::creator<Shader>::crt();
    vs->set(vertexCode[0], Shader_Type::VERTEX_SHADER);
    pipe->attach_shader(vs);

    auto geometryCode = utils::get_between(code, "#geometry", "#end");
    if (geometryCode.size())
    {
        auto gs = RenderAPI::creator<Shader>::crt();
        gs->set(geometryCode[0], Shader_Type::GEOMETRY_SHADER);
        pipe->attach_shader(gs);
    }

    auto fragmentCode = utils::get_between(code, "#fragment", "#end");
    Assert(fragmentCode.size());
    auto fs = RenderAPI::creator<Shader>::crt();
    fs->set(fragmentCode[0], Shader_Type::FRAGMENT_SHADER);
    pipe->attach_shader(fs);

    pipe->compile();

    return pipe;
}

std::unordered_map<std::string, Ref<Pipeline>> *PipelineManager::get_map()
{
    static std::unordered_map<std::string, Ref<Pipeline>> m_pipelines;

    return &m_pipelines;
}