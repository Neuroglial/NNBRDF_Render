#pragma once

#include <glm/glm.hpp>
#include "core/render/Material.hpp"
#include "core/platform/renderAPI/RenderAPI.hpp"
#include "utils/utils.hpp"
#include "scene/UniformManager.hpp"
#include "shaders/uniform/UB_Lights.hpp"

class LightManager : public UniformManager<UB_Lights_t, UB_Lights_Binding>
{
public:
    static void init();

    static void Clear();

    static bool AddLight(const DirectionalLight_t &direct);

    static bool AddLight(const PointLight_t &point);

    static bool AddLight(const SpotLight_t &spot);

    static float GetPointLightFarPlane(const PointLight_t *point);

    static void RenderPointLightShadowMap(Ref<TextureCube> &shadowMap, PointLight_t *point, SceneManager *sceneMgr);

    static void RenderShadowMap(SceneManager *sceneMgr);

    static void bind(Material *mat);

//private:
    inline static Ref<Material> mt_PointLight;
    inline static Ref<TextureCube> m_PointShadowDefaut;
    inline static Ref<TextureCube> m_PointShadow[MAX_POINT_LIGHTS];
};