#include "scene/Component.hpp"
#include "scene/GameObject.hpp"

void TransformComponent::attach(TransformComponent *father)
{
    if (father && father != this)
    {
        m_father = father->gameObject;
        father->m_children.push_back(gameObject);
    }
}

void TransformComponent::detach()
{
    if (m_father)
    {
        auto *ft = m_father->get_component<TransformComponent>();
        if (ft)
        {
            for (auto it = ft->m_children.begin(); it != ft->m_children.end(); ++it)
            {
                if ((*it) == gameObject)
                {
                    ft->m_children.erase(it);
                }
            }
        }
    }

    m_father = nullptr;
}

glm::vec3 TransformComponent::get_world_pos()
{
    glm::vec4 wp = m_model * glm::vec4(0, 0, 0, 1);
    wp /= wp[3];
    return wp;
}

glm::vec3 TransformComponent::get_right()
{
    return utils::get_right(m_model);
}

glm::vec3 TransformComponent::get_forword()
{
    return utils::get_forword(m_model);
}

void RendererComponent::Render()
{
    auto *trans = gameObject->get_component<TransformComponent>();
    auto *mesh = gameObject->get_component<MeshComponent>();
    if (mesh && trans && mesh->m_mesh && m_materials.size())
    {
        mesh->m_mesh->draw(*m_materials[0].get(), trans->m_model);
    }
}