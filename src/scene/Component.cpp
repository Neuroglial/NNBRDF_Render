#include "scene/Component.hpp"
#include "scene/GameObject.hpp"
#include "utils/utils.hpp"
#include "glm/matrix.hpp"
#include "SceneManger.hpp"
#include "core/platform/renderAPI/RenderAPI.hpp"

void TransformComponent::attach(TransformComponent *father)
{
    if (have_child(father->gameObject))
        return;

    detach();

    auto local_model = glm::inverse(father->m_model) * m_model;

    m_pos = utils::get_position(local_model);
    m_rot = utils::get_rotation(local_model);
    m_scl = utils::get_scale(local_model);

    if (father && father != this)
    {
        m_father = father->gameObject;
        father->m_children.push_back(gameObject);
    }
}

void TransformComponent::detach()
{
    m_pos = utils::get_position(m_model);
    m_rot = utils::get_rotation(m_model);
    m_scl = utils::get_scale(m_model);

    if (m_father)
    {
        auto *ft = m_father->get_component<TransformComponent>();
        if (ft)
        {
            for (int i = 0; i < ft->m_children.size(); ++i)
            {
                if (ft->m_children[i] == gameObject)
                {
                    ft->m_children[i] = ft->m_children.back();
                    ft->m_children.pop_back();
                    break;
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

void TransformComponent::get_allChildren(std::vector<GameObject *> &children)
{
    children.insert(children.end(), m_children.begin(), m_children.end());
    for (auto child : m_children)
    {
        auto *ct = child->get_component<TransformComponent>();
        if (ct)
            ct->get_allChildren(children);
    }
}

bool TransformComponent::have_child(GameObject *object)
{
    for (auto child : m_children)
    {
        if (object == child)
            return true;

        auto *ct = child->get_component<TransformComponent>();

        if (ct && ct->have_child(object))
            return true;
    }

    return false;
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

glm::vec3 CameraComponet::get_pos()
{
    if (auto *trans = gameObject->get_component<TransformComponent>())
    {
        return trans->m_pos;
    }
    return glm::vec3(0);
};

glm::mat4 CameraComponet::get_proj()
{
    glm::vec2 aspect = RenderAPI::get_frameBufferSize();

    return glm::perspective(m_fov, aspect.x / aspect.y, m_near, m_far);
}
