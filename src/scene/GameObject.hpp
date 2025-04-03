#pragma once

#include <entt/entt.hpp>
#include "utils/utils.hpp"
#include "scene/Component.hpp"
#include "scene/ScriptManager.hpp"

template <typename T, typename... Args>
inline T &add_component_rt(entt::entity id, entt::registry *reg, Args &&...rest)
{
    return reg->emplace<T>(id, rest...);
}

class SceneManager;

class GameObject
{
public:
    GameObject() = delete;

    GameObject(SceneManager *sceneMgr, EventManager *eventMgr, entt::registry *scene, entt::entity id, const std::string &name = "") : m_sceneMgr(sceneMgr), m_eventMgr(eventMgr), m_scene(scene), m_id(id), m_name(name) {}

    void destroy();

    const std::string &get_name() const
    {
        return m_name;
    }

    void set_name(const std::string &name)
    {
        m_name = name;
    }

    template <typename T, typename... Args>
    T &add_component(Args &&...rest)
    {
        T &comp = add_component_rt<T>(m_id, m_scene, std::forward<Args>(rest)...);
        comp.gameObject = this;
        return comp;
    }

    template <typename T>
    void remove_component()
    {
        m_scene->remove<T>(m_id);
    }

    template <typename T>
    T *get_component() const
    {
        return m_scene->try_get<T>(m_id);
    }

    entt::entity get_id()
    {
        return m_id;
    }

    void attach(GameObject *father, bool trans = true)
    {
        if (father && father != this && father->m_sceneMgr == m_sceneMgr)
        {
            auto *f_trans = father->get_component<TransformComponent>();
            auto *trans = get_component<TransformComponent>();

            if (f_trans && trans)
            {
                trans->detach();
                trans->attach(f_trans, trans);
            }
        }
    }

    void detach()
    {
        auto *trans = get_component<TransformComponent>();
        if (trans)
        {
            trans->detach();
        }
    }

    SceneManager *get_scene()
    {
        return m_sceneMgr;
    }

private:
    SceneManager *m_sceneMgr;
    EventManager *m_eventMgr;
    entt::registry *m_scene;
    entt::entity m_id;

    std::string m_name;
    friend struct ComponentBase;
};