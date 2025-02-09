#pragma once

#include "core/platform/system/Event.hpp"
#include "glm/glm.hpp"
#include "utils/utils.hpp"

class Object
{
public:
    virtual void tick(float delta) {};
    virtual void callback(Event::Event &_event) {};
    virtual void on_construct() {};
};

class Actor : public Object
{
public:
    Actor(const glm::vec3 &position = glm::vec3(0.0f), const glm::vec3 &rotation = glm::vec3(0.0f), const glm::vec3 &scale = glm::vec3(1.0f)) : m_position(position), m_rotation(rotation), m_scale(scale) {}

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
        // glm::mat4 rotation = utils::get_rotation(m_rotation);
        // glm::vec4 forward = rotation * glm::vec4(0.0f, 0.0f, -1.0f, 0.0f);
        // return forward;

        return utils::get_forword(utils::get_model(m_position, m_scale, m_rotation));
    }

    glm::vec3 get_right()
    {
        // glm::mat4 rotation = utils::get_rotation(m_rotation);
        // glm::vec4 forward = rotation * glm::vec4(1.0f, 0.0f, 0.0f, 0.0f);
        // return forward;

        return utils::get_right(utils::get_model(m_position, m_scale, m_rotation));
    }

protected:
    glm::vec3 m_position;
    glm::vec3 m_rotation;
    glm::vec3 m_scale;
};