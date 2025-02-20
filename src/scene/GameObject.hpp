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

template <>
inline ScriptComponent &add_component_rt<ScriptComponent, std::string>(entt::entity id, entt::registry *reg, std::string &&name)
{
    auto &ret = reg->emplace<ScriptComponent>(id);
    ret.script = ScriptManager::create(name, reg);
    return ret;
}

class SceneManager;

class GameObject
{
public:
    GameObject() = delete;

    GameObject(SceneManager *sceneMgr, EventManager *eventMgr, entt::registry *scene, entt::entity id, const std::string &name = "") : m_sceneMgr(sceneMgr), m_eventMgr(eventMgr), m_scene(scene), m_id(id), m_name(name) {}

    void destroy();

    const std::string &get_name()
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

        if constexpr (std::is_same_v<ScriptComponent, T>)
        {
            comp.script->gameObject = this;
            if (comp.script)
                m_eventMgr->registerCallback(std::bind(&ScriptBase::CallBack, comp.script.get(), std::placeholders::_1));
        }

        return comp;
    }

    template <typename T>
    T *get_component()
    {
        return m_scene->try_get<T>(m_id);
    }

    entt::entity get_id()
    {
        return m_id;
    }

    void attach(GameObject *father)
    {
        if (father && father != this && father->m_sceneMgr == m_sceneMgr)
        {
            auto *f_trans = father->get_component<TransformComponent>();
            auto *trans = get_component<TransformComponent>();

            if (f_trans && trans)
            {
                trans->detach();
                trans->attach(f_trans);
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

private:
    SceneManager *m_sceneMgr;
    EventManager *m_eventMgr;
    entt::registry *m_scene;
    entt::entity m_id;

    std::string m_name;
};