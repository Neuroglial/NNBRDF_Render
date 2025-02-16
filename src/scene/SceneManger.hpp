#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "utils/utils.hpp"
#include "scene/GameObject.hpp"
#include "scene/ScriptManager.hpp"
#include "scene/Component.hpp"

#include "core/platform/system/EventManager.hpp"

class SceneManager
{
public:
    SceneManager(EventManager &eventMgr) : m_lastObjectIndex(0), m_eventMgr(&eventMgr)
    {
        m_disID = eventMgr.registerDistributor(m_distributor);
    }

    ~SceneManager()
    {
        OnDestroy();
        m_eventMgr->deleteCallback(m_disID);
    }

    Ref<GameObject> create_Object()
    {
        return create_Object("GameObject_New_" + std::to_string(m_lastObjectIndex++));
    }

    Ref<GameObject> create_Object(const std::string &name)
    {
        auto id = m_registry.create();
        auto obj = std::make_shared<GameObject>(this, &m_distributor, &m_registry, id, name);

        obj->add_component<TransformComponent>();
        m_objects.push_back(obj);

        return obj;
    }

    const std::vector<TransformComponent *> &get_root()
    {
        return m_root;
    }

    void delete_object(GameObject *object)
    {
        m_delete.push_back(object);
    }

    void Update(float delta)
    {
        update_script(delta);
        update_camera();
        update_delete();
        update_transform();
        update_light();
    }

    void Start()
    {
        m_registry.view<ScriptComponent>().each(
            [](ScriptComponent &sc)
            {
                if (sc.script)
                    sc.script->Start();
            });
    }

    void OnDestroy()
    {
        m_registry.view<ScriptComponent>().each(
            [](ScriptComponent &sc)
            {
                if (sc.script)
                    sc.script->OnDestroy();
            });
    }

    entt::registry *get_reg()
    {
        return &m_registry;
    }

    void render_mesh(Material *mat = nullptr)
    {
        if (mat)
        {
            m_registry.view<TransformComponent, MeshComponent>().each(
                [mat](TransformComponent &trans, MeshComponent &mesh)
                {
                    if (mesh.m_mesh)
                        mesh.m_mesh->draw(*mat, trans.m_model);
                });
        }
        else
        {
            m_registry.view<RendererComponent>().each(
                [mat](RendererComponent &renderer)
                {
                    renderer.Render();
                });
        }
    };

    void render_shadow(Material *mat)
    {
        m_registry.view<TransformComponent, MeshComponent>().each(
            [mat](TransformComponent &trans, MeshComponent &mesh)
            {
                if (mesh.m_mesh && mesh.m_castShadow)
                    mesh.m_mesh->draw(*mat, trans.m_model);
            });
    };

private:
    void update_delete()
    {
        for (auto object : m_delete)
        {
            if (auto *srp = object->get_component<ScriptComponent>())
                srp->OnDestroy();
        }

        for (auto object : m_delete)
        {
            m_registry.destroy(object->get_id());
            for (auto i = m_objects.begin(); i != m_objects.end(); ++i)
            {
                if ((*i).get() == object)
                {
                    m_objects.erase(i);
                    break;
                }
            }
        }

        m_delete.clear();
    }

    void update_script(float delta)
    {
        m_registry.view<ScriptComponent>().each(
            [delta](ScriptComponent &sc)
            {
                if (sc.script)
                    sc.script->Update(delta);
            });
    }

    void update_camera()
    {
        m_registry.view<TransformComponent, CameraComponet>().each(
            [](TransformComponent &trans, CameraComponet &camera)
            {
                // The default camera is facing the negative Z-axis direction. To match intuition,
                //  rotate 180 degrees to make it face the positive Z-axis direction
                const static glm::mat4 rot180_Y = utils::get_rotation(glm::vec3(0, 180, 0));
                camera.m_view = rot180_Y * glm::inverse(trans.m_model);
            });
    }

    void update_light()
    {
        LightManager::Clear();

        m_registry.view<TransformComponent, PointLightComponent>().each(
            [](TransformComponent &trans, PointLightComponent &point)
            {
                point.m_data.position = trans.get_world_pos();
                LightManager::AddLight(point.m_data);
            });

        LightManager::RenderShadowMap(this);
        LightManager::UpdataBuffer();
    }

    void update_transform()
    {
        std::function<void(GameObject *, glm::mat4 &)> dp;
        dp = [this, &dp](GameObject *obj, glm::mat4 &father_trans)
        {
            auto &trans = m_registry.get<TransformComponent>(obj->get_id());
            trans.m_model = utils::get_model(trans.m_position, trans.m_scale, trans.m_rot, father_trans);
            for (auto i : trans.m_children)
            {
                dp(i, trans.m_model);
            }
        };

        m_root.clear();
        m_registry.view<TransformComponent>().each(
            [&dp, this](TransformComponent &trans)
            {
                if (trans.m_father == nullptr)
                {
                    m_root.push_back(&trans);

                    if (!trans.m_static || trans.m_model == glm::mat4(0))
                    {
                        dp(trans.gameObject, glm::mat4(1));
                    }
                }
            });
    };

private:
    entt::registry m_registry;
    std::vector<Ref<GameObject>> m_objects;
    std::vector<TransformComponent *> m_root;
    std::vector<GameObject *> m_delete;

    uint32_t m_lastObjectIndex;
    EventManager m_distributor;
    Event_ID m_disID;
    EventManager *m_eventMgr;
};