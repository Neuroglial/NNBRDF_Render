#include "scene/ShaderManager.hpp"
std::unordered_map<std::string,Shader::Shader*> ShaderManager::m_shaders;
#define __SHADER_MANAGER__
#include "shaders/shader.hpp"

Shader::Shader * ShaderManager::get(const std::string &path)
{
    return m_shaders[path];
}

void ShaderManager::register_shader(const std::string &path, Shader::Shader *shader)
{
    m_shaders.insert(std::pair(path,shader));
}