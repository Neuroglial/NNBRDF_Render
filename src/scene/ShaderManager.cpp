#include "scene/ShaderManager.hpp"

ShaderManager ShaderManager::instance;

Shader::Shader * ShaderManager::get(const std::string &path)
{
    return m_shaders[path];
}

void ShaderManager::register_shader(const std::string &path, Shader::Shader *shader)
{
    m_shaders[path] = shader;
}