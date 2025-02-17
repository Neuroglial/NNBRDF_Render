#pragma once

#include <glm/glm.hpp>
#include "core/platform/renderAPI/RenderAPI.hpp"
#include "utils/utils.hpp"

#include "scene/SceneManger.hpp"
#include "shaders/uniform/UB_Camera.hpp"

#include "scene/UniformManager.hpp"
#include "scene/Component.hpp"

class CameraUniform : public UniformManager<UB_Camera, UB_Camera_Binding>
{
public:
    static void bind(CameraComponet *camera)
    {
        auto &data = GetData();
        data.view = camera->m_view;
        data.viewPos = camera->get_pos();
        data.projection = camera->get_proj();
        UpdataBuffer();
    };
};