#pragma once

#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "utils/utils.hpp"
#include "scene/GameObject.hpp"
#include "scene/ScriptManager.hpp"
#include "scene/Component.hpp"

class SceneManager
{
public:
    SceneManager() : m_lastObjectIndex(0)
    {
        m_root = create_Object("Scene Root");
    }

    ~SceneManager()
    {
        OnDestroy();
    }

    Ref<GameObject> create_Object()
    {
        return create_Object("GameObject_New_" + std::to_string(m_lastObjectIndex++));
    }

    Ref<GameObject> create_Object(const std::string &name)
    {
        auto id = m_registry.create();
        auto obj = std::make_shared<GameObject>(this, &m_registry, id, name);

        obj->add_component<TransformComponent>();
        m_objects.push_back(obj);

        if (m_root)
        {
            obj->attach(m_root.get());
        }

        return obj;
    }

    void delete_object(GameObject *object)
    {
        if (auto *srp = object->get_component<ScriptComponent>())
            srp->OnDestroy();

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

    void Update(float delta)
    {
        update_script(delta);
        update_transform();
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

    GameObject *get_root()
    {
        return m_root.get();
    }

private:
    void update_script(float delta)
    {
        m_registry.view<ScriptComponent>().each(
            [delta](ScriptComponent &sc)
            {
                if (sc.script)
                    sc.script->Update(delta);
            });
    }

    void update_transform()
    {
        std::function<void(GameObject *, glm::mat4 &)> dp;
        dp = [this, &dp](GameObject *obj, glm::mat4 &father_trans)
        {
            auto &trans = m_registry.get<TransformComponent>(obj->get_id());
            trans.m_model = utils::get_model(trans.m_pos, trans.m_scl, trans.m_rot, father_trans);
            for (auto i : trans.m_children)
            {
                dp(i, trans.m_model);
            }
        };

        m_registry.view<TransformComponent>().each(
            [&dp](TransformComponent &trans)
            {
                if (trans.m_father == nullptr && (!trans.m_static || trans.m_model == glm::mat4(0)))
                {
                    dp(trans.gameObject, glm::mat4(1));
                }
            });
    };

private:
    entt::registry m_registry;
    std::vector<Ref<GameObject>> m_objects;

    Ref<GameObject> m_root;
    uint32_t m_lastObjectIndex;
};