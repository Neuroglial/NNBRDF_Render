#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "utils/utils.hpp"
#include "scene/GameObject.hpp"
#include "scene/ScriptManager.hpp"
#include "scene/Component.hpp"

class Scene_Node
{
private:
    glm::vec3 m_pos = glm::vec3(0);
    glm::vec3 m_scl = glm::vec3(1);
    glm::vec3 m_rot = glm::vec3(0);

    glm::mat4 m_model = glm::mat4(1);
    bool m_change = false;
    Scene_Node *m_father = nullptr;
    std::vector<Scene_Node *> m_children;

    void add_child(Scene_Node *child)
    {
        m_children.push_back(child);
    }

    void remove_child(Scene_Node *child)
    {
        for (int i = 0; i < m_children.size(); ++i)
        {
            if (m_children[i] == child)
            {
                m_children[i] = m_children.back();
                m_children.resize(m_children.size() - 1);
                break;
            }
        }
    }

public:
    glm::vec3 get_loc_pos()
    {
        return m_pos;
    }

    glm::vec3 get_loc_scl()
    {
        return m_scl;
    }

    glm::vec3 get_loc_rot()
    {
        return m_rot;
    }

    glm::mat4 get_loc_model()
    {
        if (m_change)
            m_model = utils::get_model(m_pos, m_scl, m_rot);
        m_change = false;

        return m_model;
    }

    void set_loc_pos(const glm::vec3 &pos)
    {
        m_change = true;
        m_pos = pos;
    }

    void set_loc_scl(const glm::vec3 &scl)
    {
        m_change = true;
        m_scl = scl;
    }

    void set_loc_rot(const glm::vec3 &rot)
    {
        m_change = true;
        m_rot = rot;
    }

    glm::vec3 get_world_pos()
    {
        if (m_father)
        {
            glm::vec4 pos(m_pos, 1);
            pos = m_father->get_world_model() * pos;
            pos /= pos[3];
            return pos;
        }
        return m_pos;
    }

    glm::mat4 get_world_model()
    {
        if (m_father)
            return m_father->get_world_model() * get_loc_model();

        return get_loc_model();
    }

    void attach(Scene_Node &father)
    {
        m_father = &father;
        father.add_child(this);
    }

    void detach()
    {
        if (!m_father)
            return;

        m_father->remove_child(this);
        m_father = nullptr;
    }

    ~Scene_Node()
    {
        if (m_father)
        {
            for (auto i : m_children)
            {
                i->attach(*m_father);
            }

            detach();
        }
        else
        {
            for (auto i : m_children)
            {
                i->detach();
            }
        }
    }
};

class SceneManager
{
public:
    ~SceneManager()
    {
        OnDestroy();
    }

    Ref<GameObject> create_Object()
    {
        auto id = m_registry.create();
        auto obj = std::make_shared<GameObject>(this, &m_registry, id);
        obj->add_component<TransformComponent>();
        m_objects.push_back(obj);
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
};