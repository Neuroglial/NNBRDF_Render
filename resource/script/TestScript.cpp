#include "scene/ScriptManager.hpp"
#include <iostream>

class TestScript : public ScriptBase
{
    virtual void Start() override
    {
        //std::cout << "hello world" << std::endl;
    }

    virtual void Update(float delta) override
    {
        //std::cout << "tick " + std::to_string(delta) << std::endl;
    }
};
REGISTER_SCRITPT(TestScript)