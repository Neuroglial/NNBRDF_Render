// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "core/platform/system/Event.hpp"
#include "core/platform/system/EventManager.hpp"
#include "utils/utils.hpp"
#include <entt/entt.hpp>
#include <functional>

class SceneManager;

class Object_t
{
public:
    virtual void tick(float delta) {};
    virtual void callback(Event::Event &_event) {};
    virtual void on_construct() {};
};



class Actor_t : public Object_t
{
public:
    Actor_t(const glm::vec3 &position = glm::vec3(0.0f), const glm::vec3 &rotation = glm::vec3(0.0f), const glm::vec3 &scale = glm::vec3(1.0f)) : m_position(position), m_rotation(rotation), m_scale(scale) {}

    glm::vec3 &get_position()
    {
        return m_position;
    }

    glm::vec3 &get_scale()
    {
        return m_scale;
    }

    glm::vec3 &get_rotation()
    {
        return m_rotation;
    }

    glm::mat4 get_model()
    {
        return utils::get_model(m_position, m_scale, m_rotation);
    }

    glm::vec3 get_forward()
    {
        glm::mat4 rotation = utils::get_rotation(m_rotation);
        glm::vec4 forward = rotation * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
        return forward;
    }

    glm::vec3 get_right()
    {
        glm::mat4 rotation = utils::get_rotation(m_rotation);
        glm::vec4 forward = rotation * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
        return forward;
    }

protected:
    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;
};