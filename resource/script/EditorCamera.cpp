#include "scene/ScriptManager.hpp"
#include "scene/GameObject.hpp"
#include <iostream>

class EditorCamera : public ScriptBase
{
    glm::vec3 moveDir = glm::vec3(0);

    bool lockDir = true;

    float speed = 5.0f;

    void keyboardInput(KeyCode code, PressType type, Event::Event_Keyboard *event)
    {
        switch (code)
        {
        case KeyCode::W:
            if (type == PressType::Press)
                moveDir += utils::forward();
            else if (type == PressType::Release)
                moveDir -= utils::forward();
            break;

        case KeyCode::A:
            if (type == PressType::Press)
                moveDir -= utils::right();
            else if (type == PressType::Release)
                moveDir += utils::right();
            break;

        case KeyCode::S:
            if (type == PressType::Press)
                moveDir -= utils::forward();
            else if (type == PressType::Release)
                moveDir += utils::forward();
            break;

        case KeyCode::D:
            if (type == PressType::Press)
                moveDir += utils::right();
            else if (type == PressType::Release)
                moveDir -= utils::right();
            break;

        case KeyCode::MouseRight:
            if (type == PressType::Press)
                lockDir = false;
            else if (type == PressType::Release)
                lockDir = true;
            break;

        default:
            break;
        }
    }

    void mouseMove(float x, float y, Event::Event_Mouse_Move *event)
    {
        if (lockDir)
            return;

        if (auto *trans = gameObject->get_component<TransformComponent>())
        {
            auto rot = trans->get_rotEuler();

            // x *= 0.25f;
            // y *= 0.25f;

            y -= rot.x;
            y = y > 85.0f ? 85.0f : y;
            y = y < -85.0f ? -85.0f : y;

            trans->rotate_local(glm::vec3(y - rot.x, 0, 0));
            trans->rotate_world(glm::vec3(0, -x, 0));
        }
    }

    virtual void Start() override
    {
        std::cout << "Editor Camera Loaded" << std::endl;
    }

    virtual void Update(float delta) override
    {
        if (auto *trans = gameObject->get_component<TransformComponent>())
        {
            if (glm::length(moveDir) > 0.1f)
                trans->m_pos += (speed * delta * glm::normalize(moveDir));
        }
    }

    virtual void OnDestroy() override
    {
        std::cout << "Editor Camera Deleted" << std::endl;
    }

    virtual void CallBack(Event::Event &event) override
    {
        if (auto *event_ptr = dynamic_cast<Event::Event_Keyboard *>(&event))
            keyboardInput(event_ptr->m_code, event_ptr->m_pressType, event_ptr);

        else if (auto *event_ptr = dynamic_cast<Event::Event_Scroll *>(&event))
        {
            speed += event_ptr->m_yoffset * 0.1f;
            speed = std::max(0.0f, speed);
        }

        else if (auto *event_ptr = dynamic_cast<Event::Event_Mouse_Move *>(&event))
        {
            mouseMove(event_ptr->m_xDelta, event_ptr->m_yDelta, event_ptr);
        }
    }
};
REGISTER_SCRITPT(EditorCamera)