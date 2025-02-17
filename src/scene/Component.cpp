#include "scene/Component.hpp"
#include "scene/GameObject.hpp"
#include "utils/utils.hpp"
#include "glm/matrix.hpp"
#include "SceneManger.hpp"
#include "core/platform/renderAPI/RenderAPI.hpp"

#include "editor/ImGui.hpp"

#define DRAW_INSPECTOR(x)                   \
    if (auto cmp = obj->get_component<x>()) \
    cmp->DrawInspector()

void DrawInspector(GameObject *obj)
{
    DRAW_INSPECTOR(TransformComponent);
    DRAW_INSPECTOR(CameraComponet);
    DRAW_INSPECTOR(MeshComponent);
    DRAW_INSPECTOR(RendererComponent);
    DRAW_INSPECTOR(PointLightComponent);
    DRAW_INSPECTOR(ScriptComponent);
}

void TransformComponent::attach(TransformComponent *father)
{
    if (have_child(father->gameObject))
        return;

    detach();

    auto local_model = glm::inverse(father->m_model) * m_model;

    m_position = utils::get_position(local_model);
    m_rot = utils::get_rotation(local_model);
    m_scale = utils::get_scale(local_model);

    if (father && father != this)
    {
        m_father = father->gameObject;
        father->m_children.push_back(gameObject);
    }
}

void TransformComponent::detach()
{
    m_position = utils::get_position(m_model);
    m_rot = utils::get_rotation(m_model);
    m_scale = utils::get_scale(m_model);

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

void TransformComponent::set_world_pos(glm::vec3 pos)
{
    glm::vec4 trans(pos, 1);
    trans = glm::inverse(m_model) * trans;
    trans /= trans[3];
    m_position += glm::vec3(trans);
}

glm::vec3 TransformComponent::get_right()
{
    return utils::get_right(m_model);
}

glm::vec3 TransformComponent::get_forward()
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

void TransformComponent::DrawInspector()
{
    UI::PushID();
    ImGui::Text("Transform:");
    UI::DrawVec3Control("Position", m_position);
    {
        auto rot = get_rotEuler();
        auto rotBefore = rot;
        UI::DrawVec3Control("Rotation", rot);
        rotate_local(rot - rotBefore);
    }
    UI::DrawVec3Control("Scale", m_scale, 1.0f, 0.0f);
    ImGui::NewLine();
    UI::PopID();
}

void MeshComponent::DrawInspector()
{
    UI::PushID();
    ImGui::Text("Mesh Fliter:");
    UI::Property("Cast Shadow", m_castShadow);
    ImGui::NewLine();
    UI::PopID();
}

void RendererComponent::Render()
{
    auto *trans = gameObject->get_component<TransformComponent>();
    auto *mesh = gameObject->get_component<MeshComponent>();
    if (mesh && trans && mesh->m_mesh && m_materials.size())
    {
        LightManager::bind(m_materials[0].get());
        mesh->m_mesh->draw(*m_materials[0].get(), trans->m_model);
    }
}

void RendererComponent::DrawInspector()
{
    UI::PushID();
    ImGui::Text("Renderer:");

    if (m_materials.size())
    {
        auto &mat = m_materials[0];
        auto list = mat->get_params_list();

        for (auto& i : list->m_param_list)
        {
            switch (i.second.m_type)
            {
            case ShaderParam_Type::Int:
            {
                if (!i.second.m_value_ptr)
                    break;

                int *value = (int *)i.second.m_value_ptr;
                UI::Property(i.first.c_str(), *value);
                break;
            }

            case ShaderParam_Type::Float:
            {
                if (!i.second.m_value_ptr)
                    break;

                float *value = (float *)i.second.m_value_ptr;
                UI::Property(i.first.c_str(), *value);
                break;
            }

            case ShaderParam_Type::Vec2:
            {
                if (!i.second.m_value_ptr)
                    break;

                glm::vec2 *value = (glm::vec2 *)i.second.m_value_ptr;
                UI::Property(i.first.c_str(), *value);
                break;
            }

            case ShaderParam_Type::Vec3:
            {
                if (!i.second.m_value_ptr)
                    break;

                glm::vec3 *value = (glm::vec3 *)i.second.m_value_ptr;
                UI::PropertyColor(i.first.c_str(), *value);
                break;
            }

            case ShaderParam_Type::Vec4:
            {
                if (!i.second.m_value_ptr)
                    break;

                glm::vec4 *value = (glm::vec4 *)i.second.m_value_ptr;
                UI::Property(i.first.c_str(), *value);
                break;
            }

            default:
                break;
            }
        }
    }

    ImGui::NewLine();
    UI::PopID();
}

void PointLightComponent::DrawInspector()
{
    UI::PushID();
    ImGui::Text("Point Light:");
    UI::PropertyColor("Color", m_data.color);
    UI::Property("Intensity", m_data.intensity);
    m_data.intensity = glm::max(m_data.intensity, 0.0f);

    bool renderShadow = m_data.ptMapIndex;
    UI::Property("Render Shadow", renderShadow);
    m_data.ptMapIndex = renderShadow;

    UI::Property("Redius", m_data.radius);
    ImGui::NewLine();
    UI::PopID();
}

glm::vec3 CameraComponet::get_pos()
{
    if (auto *trans = gameObject->get_component<TransformComponent>())
    {
        return trans->m_position;
    }
    return glm::vec3(0);
};

glm::mat4 CameraComponet::get_proj()
{
    glm::vec2 aspect = RenderAPI::get_frameBufferSize();

    return glm::perspective(glm::radians(m_fov), aspect.x / aspect.y, m_near, m_far);
}

void CameraComponet::DrawInspector()
{
    UI::PushID();
    ImGui::Text("Camera:");
    UI::Property("Fov", m_fov);
    UI::Property("Near Plane", m_near);
    UI::Property("Far Plane", m_far);
    ImGui::NewLine();
    UI::PopID();
}
