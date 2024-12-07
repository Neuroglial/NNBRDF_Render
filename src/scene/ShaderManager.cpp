#include "scene/ShaderManager.hpp"

/*
    shader的注册需要用到shader manager,下面的顺序不能颠倒,
    不然shader先调用构造函数,manager还没有初始化会发生错误
*/

std::unordered_map<std::string,Shader*> ShaderManager::m_shaders;
#define __SHADER_MANAGER__
#include "shaders/shaders.hpp"


Shader * ShaderManager::get(const std::string &path)
{
    auto i = m_shaders.find(path);

    if(i == m_shaders.end())
        throw std::runtime_error("Shader Named " + path + " Don't Found");

    return i->second;
}

void ShaderManager::register_shader(const std::string &path, Shader *shader)
{
    m_shaders.insert(std::pair<std::string,Shader*>(path,shader));
}