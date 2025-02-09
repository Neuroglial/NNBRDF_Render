#pragma once

#include <entt/entt.hpp>
#include "utils/utils.hpp"

class GameObject
{
public:
    GameObject() = delete;

    GameObject(entt::entity id) : m_id(id) {}

    entt::entity get_id()
    {
        return m_id;
    }

private:
    entt::entity m_id;
};