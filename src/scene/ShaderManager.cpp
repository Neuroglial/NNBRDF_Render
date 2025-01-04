// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#include "scene/ShaderManager.hpp"

std::unordered_map<std::string, Shader_Info> *ShaderManager::get_map()
{
    static std::unordered_map<std::string, Shader_Info> shaders;
    return &shaders;
}

Ref<Shader> ShaderManager::get(const std::string &name)
{
    return get_info(name).m_Shader;
}

Shader_Info &ShaderManager::get_info(const std::string &name)
{
    auto i = get_map()->find(name);

    if (i == get_map()->end())
        throw std::runtime_error("Shader Named " + name + " Don't Found");

    if (i->second.m_Shader == nullptr)
        i->second.reload();

    return i->second;
}

bool ShaderManager::is_exist(const std::string &name)
{
    auto i = get_map()->find(name);

    if (i == get_map()->end())
        return false;
    return true;
}

void ShaderManager::register_shader(const std::string &name, const std::string &code, Shader_Type type, ShaderParamList *list)
{
    get_map()->insert(std::pair<std::string, Shader_Info>(name, Shader_Info(code, type, list)));
}