#pragma once

#include <core/platform/renderAPI/Shader.hpp>
#include <unordered_map>

class ShaderManager{
    public:
    static ShaderManager instance;

    Shader::Shader* get(const std::string& path);

    void register_shader(const std::string& path,Shader::Shader* shader);

    private:
    std::unordered_map<std::string,Shader::Shader*> m_shaders;
};