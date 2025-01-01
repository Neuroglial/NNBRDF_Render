#pragma once

#include "scene/Object_t.hpp"
#include <glm/glm.hpp>
#include "resource/shaders/shaders_uniform.hpp"
#include "core/render/Material.hpp"
#include "core/platform/renderAPI/RenderAPI.hpp"
#include "utils/utils.hpp"

enum class Light_Type
{
    None,
    Point,
    Direct,
    Spot,
};

class Light
{
public:
    Light(Light_Type type) : m_type(type) {}

    Light_Type get_type()
    {
        return m_type;
    }

    virtual void buffer_update(glm::vec3 &position, glm::vec3 &direction) = 0;
    virtual void bind_light(Material &mat) = 0;

    glm::vec3 m_ambient = glm::vec3(0.1);
    glm::vec3 m_diffuse = glm::vec3(0.75);
    glm::vec3 m_specular = glm::vec3(1.0f);

protected:
    Light_Type m_type = Light_Type::None;
    int m_index = -1;
};

class LightManager
{
    
};

class PointLight : public Light
{
public:
    PointLight() : Light(Light_Type::Point), m_render_lightmap(false)
    {
        if (ub_lights_data.point_num >= NR_POINT_LIGHTS_MAX)
            --ub_lights_data.point_num;
        m_index = ub_lights_data.point_num++;
    }

    virtual void buffer_update(glm::vec3 &position, glm::vec3 &direction)
    {
        ub_lights_data.point_lights[m_index].constant = m_constant / m_strength;
        ub_lights_data.point_lights[m_index].linear = m_linear / m_strength;
        ub_lights_data.point_lights[m_index].quadratic = m_quadratic / m_strength;

        ub_lights_data.point_lights[m_index].position = position;
        ub_lights_data.point_lights[m_index].ambient = m_ambient;
        ub_lights_data.point_lights[m_index].diffuse = m_diffuse;
        ub_lights_data.point_lights[m_index].specular = m_specular;
    }

    virtual void bind_light(Material &mat)
    {
    }

    float m_constant = 1.0f;
    float m_linear = 0.09f;
    float m_quadratic = 0.032f;
    float m_strength = 1.0f;
    bool m_render_lightmap;
    Ref<TextureCube> m_lightmap;
};