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

std::unordered_map<std::string, Ref<Pipeline>> *PipelineManager::get_map()
{
    static std::unordered_map<std::string, Ref<Pipeline>> m_pipelines;

    return &m_pipelines;
}