// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#pragma once

#include "core/platform/renderAPI/Shader.hpp"
#include "scene/ShaderManager.hpp"

#include <glm/glm.hpp>

class Shader_GL : public Shader
{
public:
    Shader_GL(const std::string &code, Shader_Type type) : Shader(code, type) {}

    Shader_GL() : Shader() {}

    virtual bool compile() override;

    virtual ~Shader_GL();

    unsigned int get_id();

    bool is_compiled() { return m_compiled; }

private:
    unsigned int m_id = 0;
};

class Pipeline_GL : public Pipeline
{
public:
    Pipeline_GL() : m_id(0), m_texture_index(0)
    {
    }

    virtual void bind() override;

    virtual void set_params(SD_ParamList &params) override;

    virtual void set_params(const std::string &name, SD_Param &param) override;

    virtual Ref<SD_ParamList> get_params_list() override;

    virtual Ref<Ref_Params> get_params() override;

    virtual bool compile() override;

    virtual void clear() override;

    unsigned int get_id()
    {
        return m_id;
    }

    ~Pipeline_GL();

private:
    unsigned int m_id;
    int m_texture_index;
    std::unordered_map<std::string, int> m_params_map;
};