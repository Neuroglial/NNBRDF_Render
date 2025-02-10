#include "scene/GameObject.hpp"
#include "scene/SceneManger.hpp"

void GameObject::destroy()
{
    m_sceneMgr->delete_object(this);
}