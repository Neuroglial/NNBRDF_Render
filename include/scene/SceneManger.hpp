#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "utils/utils.hpp"
#include "scene/Object.hpp"
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

template <typename T, typename... Args>
T &add_component_rt(entt::entity id, entt::registry *reg, Args &&...rest)
{
    return reg->emplace<T>(id, rest...);
};

template <>
ScriptComponent &add_component_rt<ScriptComponent, std::string>(entt::entity id, entt::registry *reg, std::string &&name)
{
    auto &ret = reg->emplace<ScriptComponent>(id);
    ret.script = ScriptManager::create(name, id, reg);
    return ret;
};

class SceneManager
{
public:
    Ref<GameObject> create_Object()
    {
        auto id = m_registry.create();
        auto obj = std::make_shared<GameObject>(id);
        m_objects.push_back(obj);
        return obj;
    }

    template <typename T, typename... Args>
    T &add_component(Ref<GameObject> obj, Args &&...rest)
    {
        return add_component_rt<T>(obj->get_id(), &m_registry, std::forward<Args>(rest)...);
    }

    void Update(float delta)
    {
        m_registry.view<ScriptComponent>().each(
            [delta](ScriptComponent &sc)
            {
                if (sc.script)
                    sc.script->Update(delta);
            });
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

    entt::registry *get_reg()
    {
        return &m_registry;
    }

private:
    entt::registry m_registry;
    std::vector<Ref<GameObject>> m_objects;
};