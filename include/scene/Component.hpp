#pragma once
#include <entt/entt.hpp>
#include "utils/utils.hpp"

class ScriptManager;

class ScriptBase
{
public:
    virtual void Start() = 0;
    virtual void Update(float delta) = 0;

private:
    template <typename T>
    T &GetComponent()
    {
        return m_reg->get<T>(m_id);
    }
    entt::entity m_id;
    entt::registry *m_reg = nullptr;
    friend class ScriptManager;
};

struct ScriptComponent
{
    Ref<ScriptBase> script;
};