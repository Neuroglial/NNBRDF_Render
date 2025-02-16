#pragma once

#include <glm/glm.hpp>
#include "core/render/Material.hpp"
#include "core/platform/renderAPI/RenderAPI.hpp"
#include "utils/utils.hpp"
#include "UniformManager.hpp"

// light structure, corresponding to shaders/uniform/UB_Lights.inc

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
    alignas(4) float cutOff;      // Cosine of an interior angle (e.g., cos(12°))
    alignas(4) float outerCutOff; // Cosine of exterior angle (e.g. cos(15°))
    alignas(4) float radius;

    alignas(4) int sptMapIndex;
};

struct UB_Lights_t
{
    alignas(16) glm::vec3 lightNum;
    alignas(16) DirectionalLight_t dirLight[MAX_DIR_LIGHTS];
    alignas(16) PointLight_t ptLight[MAX_POINT_LIGHTS];
    alignas(16) SpotLight_t sptLight[MAX_SPOT_LIGHTS];
};

class LightManager : public UniformManager<UB_Lights_t,3>
{
public:

    static void Clear()
    {
        GetData().lightNum = glm::vec3(0.0f);
    }

    static bool AddLight(const DirectionalLight_t &direct)
    {
        auto &data = GetData();
        if (data.lightNum.x < MAX_DIR_LIGHTS)
        {
            memcpy(&data.dirLight[(int)(data.lightNum.x++ + 0.5f)], &direct, sizeof(DirectionalLight_t));
            return true;
        }

        return false;
    }

    static bool AddLight(const PointLight_t &point)
    {
        auto &data = GetData();
        if (data.lightNum.y < MAX_POINT_LIGHTS)
        {
            memcpy(&data.ptLight[(int)(data.lightNum.y++ + 0.5f)], &point, sizeof(PointLight_t));
            return true;
        }

        return false;
    }

    static bool AddLight(const SpotLight_t &spot)
    {
        auto &data = GetData();
        if (data.lightNum.z < MAX_SPOT_LIGHTS)
        {
            memcpy(&data.sptLight[(int)(data.lightNum.z++ + 0.5f)], &spot, sizeof(SpotLight_t));
            return true;
        }

        return false;
    }

    static void RenderPointLightShadowMap(FrameBuffer *frameBuffer, const glm::vec3 &positon, SceneManager *sceneMgr)
    {
    }
};