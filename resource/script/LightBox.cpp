#include "scene/ScriptManager.hpp"
#include "scene/GameObject.hpp"
#include <iostream>

class LightBox : public ScriptBase
{

    virtual void Start() override
    {
    }

    virtual void Update(float delta) override
    {
        auto *light = gameObject->get_component<PointLightComponent>();
        auto *renderer = gameObject->get_component<RendererComponent>();

        if (light && renderer && renderer->m_materials.size())
        {
            renderer->m_materials[0]->set_param("lightColor", &light->m_data.color);
            renderer->m_materials[0]->set_param("intensity", &light->m_data.intensity);
        }
    }

    virtual void OnDestroy() override
    {
    }

    virtual void CallBack(Event::Event &event) override
    {
    }
};
REGISTER_SCRITPT(LightBox)