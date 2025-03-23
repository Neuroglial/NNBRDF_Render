#include "scene/ScriptManager.hpp"
#include "scene/GameObject.hpp"
#include <iostream>

class TestScript : public ScriptBase
{
    PMV(PM_Float, floatTest, 36.0f);

    double lifeTime;
    int lifeSeconds;

    virtual void Start() override
    {
        std::cout << "hello world " << floatTest << std::endl;
    }

    virtual void Update(float delta) override
    {
        lifeTime += delta;
        if (lifeTime > lifeSeconds + 1)
        {
            std::cout << "seconds tick: " + std::to_string(++lifeSeconds) << std::endl;
        }
    }

    virtual void OnDestroy() override
    {
        std::cout << "good bye " << std::endl;
    }

    virtual void CallBack(Event::Event &event) override
    {
        if (auto *mouseClick = dynamic_cast<Event::Event_Keyboard *>(&event))
        {
            if (mouseClick->m_code == KeyCode::MouseLeft)
            {
                if (mouseClick->m_pressType == PressType::Press)
                    std::cout << "Mouse Right Press" << std::endl;
                else if (mouseClick->m_pressType == PressType::Repeat)
                    std::cout << "Mouse Right Repeat" << std::endl;
                else
                    std::cout << "Mouse Right Release" << std::endl;
            }
        }
    }
};
REGISTER_SCRITPT(TestScript)