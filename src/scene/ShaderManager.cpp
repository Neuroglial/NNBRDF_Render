#include "scene/ShaderManager.hpp"

/*
    shader的注册需要用到shader manager,下面的顺序不能颠倒,
    不然shader先调用构造函数,manager还没有初始化会发生错误
*/

#define __SHADER_MANAGER__
#include "shaders/shaders.hpp"

std::unordered_map<std::string,Shader*>* ShaderManager::get_map()
{
    static std::unordered_map<std::string,Shader*> shaders;
    return &shaders;
}

Shader * ShaderManager::get(const std::string &path)
{
    auto i = get_map()->find(path);

    if(i == get_map()->end())
        throw std::runtime_error("Shader Named " + path + " Don't Found");

    return i->second;
}

void ShaderManager::register_shader(const std::string &path, Shader *shader)
{
    get_map()->insert(std::pair<std::string,Shader*>(path,shader));
}