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

    GameObject(SceneManager *sceneMgr, entt::registry *scene, entt::entity id) : m_sceneMgr(sceneMgr), m_scene(scene), m_id(id) {}

    void destroy();

    template <typename T, typename... Args>
    T &add_component(Args &&...rest)
    {
        T &comp = add_component_rt<T>(m_id, m_scene, std::forward<Args>(rest)...);
        comp.gameObject = this;

        if constexpr (std::is_same_v<ScriptComponent, T>)
        {
            comp.script->gameObject = this;
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

private:
    SceneManager *m_sceneMgr;
    entt::registry *m_scene;
    entt::entity m_id;
};