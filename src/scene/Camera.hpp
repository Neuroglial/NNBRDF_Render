// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "core/platform/system/Event.hpp"
#include "scene/Object.hpp"

enum class ProjectMode
{
    Ortho,
    Persp,
};

class Camera
{
public:
    Camera(float fov, float aspect_ratio, ProjectMode mode, float near = 0.1f, float far = 100.0f) : m_fov(fov), m_aspect_ratio(aspect_ratio), m_project(mode), m_near(near), m_far(far) {}

    glm::mat4 get_projection()
    {
        return glm::perspective(glm::radians(m_fov), m_aspect_ratio, m_near, m_far);
    }

    void set_fov(float fov)
    {
        m_fov = fov;
    }

    void set_aspect_ratio(float aspect_ratio)
    {
        m_aspect_ratio = aspect_ratio;
    }

protected:
    ProjectMode m_project;
    float m_fov;
    float m_aspect_ratio;
    float m_near;
    float m_far;
    glm::mat4 m_view;
};

class MyCamera : public Actor
{
public:
    Camera m_camera;

    MyCamera(float fov, float aspect_ratio, ProjectMode mode, float near = 0.1f, float far = 100.0f) : m_camera(fov, aspect_ratio, mode, near, far) {}

    virtual void tick(float delta) override
    {
        if (glm::length(m_move_forward) > 0.1f)
        {
            auto nor_move = glm::normalize(m_move_forward) * m_speed;
            m_position += nor_move.y * get_forward() * delta;
            m_position += nor_move.x * get_right() * delta;
        }
    };

    virtual void callback(Event::Event &_event) override
    {
        if (auto kb = dynamic_cast<Event::Event_Keyboard *>(&_event))
            move_input(*kb);
        else if (auto mm = dynamic_cast<Event::Event_Mouse_Move *>(&_event))
            mouse_move(*mm);
        else if (auto fr = dynamic_cast<Event::Event_Frame_Resize *>(&_event))
            m_camera.set_aspect_ratio((float)fr->m_width / (float)fr->m_height);
        else if (auto s = dynamic_cast<Event::Event_Scroll *>(&_event))
        {
            m_speed *= (1.0f + s->m_yoffset * 0.1f);
            if (m_speed <= 0.000001f)
                m_speed = 0.000001f;
        }
    }

    void mouse_move(Event::Event_Mouse_Move &mm)
    {
        glm::vec2 pos(mm.m_xpos, mm.m_ypos);

        if (m_mouse_right_press)
        {
            glm::vec2 delta = m_mouse_pos - pos;
            m_rotation.x += delta.y * 0.1f;
            m_rotation.y += delta.x * 0.1f;

            m_rotation.x = glm::clamp(m_rotation.x, -85.0f, 85.0f);

            if (m_rotation.y > 360.0f)
                m_rotation.y -= 360.0f;
            else if (m_rotation.y <= -360.0f)
                m_rotation.y += 360.0f;
        }

        m_mouse_pos = pos;
    }

    void move_input(Event::Event_Keyboard &kb)
    {
        switch (kb.m_code)
        {
        case KeyCode::W:
            if (kb.m_type == PressType::Press)
                m_move_forward.y += 1.0f;
            else if (kb.m_type == PressType::Release)
                m_move_forward.y -= 1.0f;
            break;

        case KeyCode::S:
            if (kb.m_type == PressType::Press)
                m_move_forward.y -= 1.0f;
            else if (kb.m_type == PressType::Release)
                m_move_forward.y += 1.0f;
            break;

        case KeyCode::A:
            if (kb.m_type == PressType::Press)
                m_move_forward.x -= 1.0f;
            else if (kb.m_type == PressType::Release)
                m_move_forward.x += 1.0f;
            break;

        case KeyCode::D:
            if (kb.m_type == PressType::Press)
                m_move_forward.x += 1.0f;
            else if (kb.m_type == PressType::Release)
                m_move_forward.x -= 1.0f;
            break;

        case KeyCode::MouseRight:
            if (kb.m_type == PressType::Press)
                m_mouse_right_press = true;
            else if (kb.m_type == PressType::Release)
                m_mouse_right_press = false;
            break;

        default:
            break;
        }
    }

private:
    // Forward is the positive y direction, right is the positive x direction
    glm::vec2 m_move_forward = glm::vec2(0.0f);
    bool m_mouse_right_press = false;
    glm::vec2 m_mouse_pos;
    float m_speed = 1.0f;
};