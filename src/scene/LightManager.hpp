#pragma once

#include "scene/Object.hpp"
#include <glm/glm.hpp>
#include "resource/shaders/uniform/shaders_uniform.hpp"
#include "core/render/Material.hpp"
#include "core/platform/renderAPI/RenderAPI.hpp"
#include "scene/Light.hpp"
#include "utils/utils.hpp"

#include "scene/SceneManger.hpp"

// light结构体，对应shaders/uniform/UB_Lights.inc

#define MAX_DIR_LIGHTS 4
#define MAX_DIR_LIGHTS_MAP 1

#define MAX_POINT_LIGHTS 4
#define MAX_POINT_LIGHTS_MAP 1

#define MAX_SPOT_LIGHTS 4
#define MAX_SPOT_LIGHTS_MAP 1

struct DirectionalLight_t
{
    alignas(16) glm::vec3 direction;
    alignas(16) glm::vec3 color;
    alignas(4) float intensity;

    alignas(4) int dirMapIndex;
};

struct PointLight_t
{
    alignas(16) glm::vec3 position;
    alignas(16) glm::vec3 color;
    alignas(4) float intensity;
    alignas(4) float radius;

    alignas(4) int ptMapIndex;
};

struct SpotLight_t
{
    alignas(16) glm::vec3 position;
    alignas(16) glm::vec3 direction;
    alignas(16) glm::vec3 color;
    alignas(4) float intensity;
    alignas(4) float cutOff;      // 内角余弦值（如 cos(12°)）
    alignas(4) float outerCutOff; // 外角余弦值（如 cos(15°)）
    alignas(4) float radius;

    alignas(4) int sptMapIndex;
};

struct LightUnion_t
{
    Light_Type type;
    union
    {
        DirectionalLight_t dir;
        PointLight_t pt;
        SpotLight_t spt;
    };
};

struct UB_Lights_t
{
    alignas(16) glm::vec3 lightNum;
    alignas(16) DirectionalLight_t dirLight[MAX_DIR_LIGHTS];
    alignas(16) PointLight_t ptLight[MAX_POINT_LIGHTS];
    alignas(16) SpotLight_t sptLight[MAX_SPOT_LIGHTS];
};

class LightManager
{
    static UB_Lights_t &GetLightData()
    {
        static UB_Lights_t lights;
        return lights;
    }

    static Ref<UniformBuffer> GetUniform()
    {
        static Ref<UniformBuffer> buffer;

        if (buffer == nullptr)
        {
            buffer = RenderAPI::creator<UniformBuffer>::crt();
            buffer->reset(sizeof(UB_Lights_t), 3);
        }

        return buffer;
    }

    static void Clear()
    {
        GetLightData().lightNum = glm::vec3(0.0f);
    }

    static bool AddLight(const LightUnion_t &lt)
    {
        auto &data = GetLightData();

        switch (lt.type)
        {
        case Light_Type::Direct:
            if (data.lightNum.x < MAX_DIR_LIGHTS)
            {
                memcpy(&data.dirLight[(int)(data.lightNum.x++ + 0.5f)], &lt.dir, sizeof(DirectionalLight_t));
                return true;
            }
            break;

        case Light_Type::Point:
            if (data.lightNum.y < MAX_POINT_LIGHTS)
            {
                memcpy(&data.ptLight[(int)(data.lightNum.y++ + 0.5f)], &lt.pt, sizeof(PointLight_t));
                return true;
            }
            break;

        case Light_Type::Spot:
            if (data.lightNum.z < MAX_SPOT_LIGHTS)
            {
                memcpy(&data.sptLight[(int)(data.lightNum.z++ + 0.5f)], &lt.spt, sizeof(SpotLight_t));
                return true;
            }
            break;

        default:
            break;
        }

        return false;
    }

    static void RenderPointLightShadowMap(FrameBuffer *frameBuffer, const glm::vec3 &positon, SceneManager *sceneMgr)
    {
    }

    static void UpdataBuffer()
    {
        GetUniform()->set_data(0, sizeof(UB_Lights_t), &GetLightData());
    }
};