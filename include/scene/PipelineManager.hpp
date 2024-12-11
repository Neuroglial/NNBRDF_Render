#pragma once
#include <unordered_map>
#include <vector>
#include "core/platform/renderAPI/Shader.hpp"
#include "utils/utils.hpp"

class PipelineManager
{
public:
    static Ref<Pipeline> get(const std::string &vs, const std::string &fs);

private:
    static std::unordered_map<std::string, Ref<Pipeline>> *get_map();
};