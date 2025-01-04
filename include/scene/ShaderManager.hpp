// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#pragma once
#include "core/platform/renderAPI/RenderAPI.hpp"
#include <unordered_map>
#include <vector>
#include "utils/utils.hpp"

struct Shader_Info
{
    Ref<Shader> m_Shader = nullptr;
    std::string m_code;
    Shader_Type m_type;
    ShaderParamList *m_list;

    Shader_Info(const std::string &code, Shader_Type type, ShaderParamList *list) : m_code(code), m_type(type), m_list(list) {}

    void reload()
    {
        m_Shader = Ref<Shader>(RenderAPI::creator<Shader>::crt());
        m_Shader->set_params_list(m_list);
        m_Shader->set(m_code, m_type);
    }
};

class ShaderManager
{
public:
    static Shader_Info &get_info(const std::string &name);
    static Ref<Shader> get(const std::string &name);
    static void register_shader(const std::string &name, const std::string &code, Shader_Type type, ShaderParamList *list);
    static bool is_exist(const std::string &name);

private:
    static std::unordered_map<std::string, Shader_Info> *get_map();
};

#define DECLARE_SHADER(cls)                                                            \
    Parameters pms;                                                                    \
    cls(const std::string &code, Shader_Type type)                                     \
    {                                                                                  \
        ShaderManager::register_shader(#cls, utils::read_from_file_with_include(code), type, &pms); \
    }

#define REGISTER_SHADER(cls, code, type) cls REGISTER_INS##cls(code, type);