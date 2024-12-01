#pragma once

#include <core/platform/renderAPI/Shader.hpp>
#include <unordered_map>
#include <vector>

class ShaderManager{
    public:
    static Shader::Shader* get(const std::string& path);
    static void register_shader(const std::string& path,Shader::Shader* shader);
    private:
    static std::unordered_map<std::string,Shader::Shader*> m_shaders;
};