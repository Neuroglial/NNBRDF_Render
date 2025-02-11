// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#include "scene/PipelineManager.hpp"
#include "core/platform/renderAPI/RenderAPI.hpp"
#include "scene/ShaderManager.hpp"

Ref<Pipeline> PipelineManager::get(const std::string &vs, const std::string &fs)
{
    std::string key(vs + "&&" + fs);
    auto mp = get_map();
    auto i = mp->find(key);

    if (i != mp->end())
        return i->second;

    auto pipe = RenderAPI::creator<Pipeline>::crt();
    pipe->attach_shader(ShaderManager::get(vs));
    pipe->attach_shader(ShaderManager::get(fs));

    mp->emplace(key, pipe);
    return pipe;
}

Ref<Pipeline> PipelineManager::get(const std::string &vs, const std::string &gs, const std::string &fs)
{
    std::string key(vs + "&&" + gs + "&&" + fs);
    auto mp = get_map();
    auto i = mp->find(key);

    if (i != mp->end())
        return i->second;

    auto pipe = RenderAPI::creator<Pipeline>::crt();
    pipe->attach_shader(ShaderManager::get(vs));
    pipe->attach_shader(ShaderManager::get(gs));
    pipe->attach_shader(ShaderManager::get(fs));

    mp->emplace(key, pipe);
    return pipe;
}

Ref<Pipeline> PipelineManager::get(const std::string &pipeline_path)
{
    auto mp = get_map();
    auto i = mp->find(pipeline_path);
    if (i != mp->end())
        return i->second;

    auto pipe = RenderAPI::creator<Pipeline>::crt();

    std::string code = utils::read_from_file_with_include(pipeline_path);
    
    size_t start = 0;
    size_t end = 0;
    std::string vert_mk = "#vertex";
    std::string frag_mk = "#fragment";
    std::string end_mk = "#end";



    if ((start = code.find(vert_mk)) != std::string::npos)
    {
        start += vert_mk.length();
        end = code.find(end_mk, start);
        if (end != std::string::npos)
        {
            ShaderManager::register_shader(pipeline_path + ".vertex", code.substr(start, end - start), Shader_Type::VERTEX_SHADER, nullptr);
        }
    }

    if ((start = code.find(frag_mk, end)) != std::string::npos)
    {
        start += frag_mk.length();
        end = code.find(end_mk, start);
        if (end != std::string::npos)
        {
            ShaderManager::register_shader(pipeline_path + ".fragment", code.substr(start, end - start), Shader_Type::FRAGMENT_SHADER, nullptr);
        }
    }

    pipe->attach_shader(ShaderManager::get(pipeline_path + ".vertex"));
    pipe->attach_shader(ShaderManager::get(pipeline_path + ".fragment"));

    mp->emplace(pipeline_path, pipe);
    return pipe;
}

std::unordered_map<std::string, Ref<Pipeline>> *PipelineManager::get_map()
{
    static std::unordered_map<std::string, Ref<Pipeline>> m_pipelines;

    return &m_pipelines;
}