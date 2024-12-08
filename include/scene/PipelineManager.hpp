#pragma once

#include <unordered_map>
#include <vector>
#include <stb_image.h>
#include "core/platform/renderAPI/Shader.hpp"
#include "utils/utils.hpp"

class PipelineManager
{
public:
    static Ref<Pipeline> get(const std::string &name)
    {
        auto i = m_pipelines.find(name);
        if (i == m_pipelines.end())
            throw std::runtime_error("Pipeline Named " + name + " Don't Exist");
        return i->second;
    }

    static void register_pipeline(std::string name, Ref<Pipeline> pipeline)
    {
        m_pipelines.insert(std::pair<std::string, Ref<Pipeline>>(name, pipeline));
    }

private:
    static std::unordered_map<std::string, Ref<Pipeline>> m_pipelines;
};