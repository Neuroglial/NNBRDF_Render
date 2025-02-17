#pragma once

#include <glm/glm.hpp>
#include "core/platform/renderAPI/RenderAPI.hpp"
#include "utils/utils.hpp"
#include "scene/SceneManger.hpp"
#include "shaders/uniform/UB_Base_Info.hpp"

class BaseInfoUniform : public UniformManager<UB_Base_Info, UB_Base_Info_Binding>
{
public:
    static void bind()
    {
        static auto lastSize = RenderAPI::get_frameBufferSize() + glm::ivec2(1);
        auto size = RenderAPI::get_frameBufferSize();
        if (lastSize != size)
        {
            GetData().iResolution = glm::vec3(size.x, size.y, 1);
            UpdataBuffer();
        }

        lastSize = size;
    };
};