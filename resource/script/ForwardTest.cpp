#include "scene/ScriptManager.hpp"
#include "scene/GameObject.hpp"
#include <iostream>

class ForwardTest : public ScriptBase
{
    virtual void Start() override
    {
    }

    virtual void Update(float delta) override
    {
        if (auto *trans = gameObject->get_component<TransformComponent>())
        {
            glm::vec3 pos = trans->get_world_pos();

            if (trans->m_children.size() > 0)
            {
                if (auto *trans1 = trans->m_children[0]->get_component<TransformComponent>())
                {
                    trans1->set_world_pos(pos + trans->get_forward());
                }
            }

            if (trans->m_children.size() > 1)
            {
                if (auto *trans1 = trans->m_children[1]->get_component<TransformComponent>())
                {
                    trans1->set_world_pos(pos + trans->get_right());
                }
            }
        }
    }

    virtual void OnDestroy() override
    {
    }

    virtual void CallBack(Event::Event &event) override
    {
    }
};
REGISTER_SCRITPT(ForwardTest)