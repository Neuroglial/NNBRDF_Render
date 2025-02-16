#include "scene/LightManager.hpp"
#include "scene/SceneManger.hpp"

void LightManager::Clear()
{
    GetData().lightNum = glm::vec3(0.0f);
}

bool LightManager::AddLight(const DirectionalLight_t &direct)
{
    auto &data = GetData();
    if (data.lightNum.x < MAX_DIR_LIGHTS)
    {
        memcpy(&data.dirLight[(int)(data.lightNum.x++ + 0.5f)], &direct, sizeof(DirectionalLight_t));
        return true;
    }

    return false;
}

bool LightManager::AddLight(const PointLight_t &point)
{
    auto &data = GetData();
    if (data.lightNum.y < MAX_POINT_LIGHTS)
    {
        memcpy(&data.ptLight[(int)(data.lightNum.y++ + 0.5f)], &point, sizeof(PointLight_t));
        return true;
    }

    return false;
}

bool LightManager::AddLight(const SpotLight_t &spot)
{
    auto &data = GetData();
    if (data.lightNum.z < MAX_SPOT_LIGHTS)
    {
        memcpy(&data.sptLight[(int)(data.lightNum.z++ + 0.5f)], &spot, sizeof(SpotLight_t));
        return true;
    }

    return false;
}

float LightManager::GetPointLightFarPlane(const PointLight_t *point)
{
    return 100.0f;
}

void LightManager::RenderPointLightShadowMap(Ref<TextureCube> &shadowMap, PointLight_t *point, SceneManager *sceneMgr)
{
    if (!mt_PointLight)
    {
        mt_PointLight = std::make_shared<Material>(Root_Path + "resource/shaders/PointLightShadowMap.glsl", true, Material::Double_Sided);
    }

    float farPlane = GetPointLightFarPlane(point);

    glm::mat4 shadowProj = glm::perspective(glm::radians(90.0f), 1.0f, SHADOW_NEAR_PLANE, farPlane);

    glm::mat4 shadowMat_0 = shadowProj * glm::lookAt(point->position, point->position + glm::vec3(1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));
    glm::mat4 shadowMat_1 = shadowProj * glm::lookAt(point->position, point->position + glm::vec3(-1.0, 0.0, 0.0), glm::vec3(0.0, -1.0, 0.0));
    glm::mat4 shadowMat_2 = shadowProj * glm::lookAt(point->position, point->position + glm::vec3(0.0, 1.0, 0.0), glm::vec3(0.0, 0.0, 1.0));
    glm::mat4 shadowMat_3 = shadowProj * glm::lookAt(point->position, point->position + glm::vec3(0.0, -1.0, 0.0), glm::vec3(0.0, 0.0, -1.0));
    glm::mat4 shadowMat_4 = shadowProj * glm::lookAt(point->position, point->position + glm::vec3(0.0, 0.0, 1.0), glm::vec3(0.0, -1.0, 0.0));
    glm::mat4 shadowMat_5 = shadowProj * glm::lookAt(point->position, point->position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, -1.0, 0.0));

    mt_PointLight->set_param("lightPos", &point->position);
    mt_PointLight->set_param("far_plane", &farPlane);
    mt_PointLight->set_param("shadowMat_0", &shadowMat_0);
    mt_PointLight->set_param("shadowMat_1", &shadowMat_1);
    mt_PointLight->set_param("shadowMat_2", &shadowMat_2);
    mt_PointLight->set_param("shadowMat_3", &shadowMat_3);
    mt_PointLight->set_param("shadowMat_4", &shadowMat_4);
    mt_PointLight->set_param("shadowMat_5", &shadowMat_5);

    auto framebuffer = RenderAPI::creator<FrameBuffer>::crt();
    framebuffer->attach(shadowMap, 0);

    framebuffer->bind(glm::vec4(0, 0, 0, 1));
    auto sdsize = framebuffer->get_size();
    RenderAPI::viewport(sdsize.x, sdsize.y);

    sceneMgr->render_shadow(mt_PointLight.get());
}

static void bind(Material *mat)
{
    
}