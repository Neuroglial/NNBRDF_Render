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

#define SHADOW_NEAR_PLANE 0.01f

struct DirectionalLight_t
{
    alignas(16) glm::vec3 direction;
    alignas(16) glm::vec3 color;
    alignas(4) float intensity;

    alignas(4) int dirMapIndex;
};

struct PointLight_t
{
    alignas(16) glm::vec3 position = glm::vec3(0.0f);
    alignas(16) glm::vec3 color = glm::vec3(0.8f);
    alignas(4) float intensity = 12.5f;
    alignas(4) float radius = 0.1f;

    alignas(4) int ptMapIndex = -1;
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

class LightManager : public UniformManager<UB_Lights_t, 3>
{
public:
    static void Clear();

    static bool AddLight(const DirectionalLight_t &direct);

    static bool AddLight(const PointLight_t &point);

    static bool AddLight(const SpotLight_t &spot);

    static float GetPointLightFarPlane(const PointLight_t *point);

    static void RenderPointLightShadowMap(Ref<TextureCube> &shadowMap, PointLight_t *point, SceneManager *sceneMgr);

    static void bind(Material *mat);

    inline static Ref<Material> mt_PointLight;
};