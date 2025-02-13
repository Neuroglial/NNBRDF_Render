#pragma once

#include "scene/Component.hpp"
#include "scene/GameObject.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace UI
{
    inline static const float floatMin = -FLT_MAX;
    inline static const float floatMax = FLT_MAX;

    void RenderInputFloat4(const std::string &name, glm::vec4 &value)
    {
        ImGui::PushID(&value);
        ImGui::Text(name.c_str());
        ImGui::NextColumn();
        ImGui::InputFloat4("", &value[0], "%.1f");
        ImGui::PopID();
    }

    void RenderInputFloat3(const std::string &name, glm::vec3 &value)
    {
        ImGui::PushID(&value);
        ImGui::Text(name.c_str());
        ImGui::NextColumn();
        ImGui::InputFloat3("", &value[0], "%.1f");
        ImGui::PopID();
    }

    void RenderDragFloat3(const std::string &name, glm::vec3 &value,
                          float speed = 0.1f, float min = floatMin, float max = floatMax, const char *format = "%.1f")
    {
        ImGui::PushID(&value);
        ImGui::Text(name.c_str());
        ImGui::NextColumn();
        ImGui::DragFloat3("", &value[0], speed, min, max, format);
        ImGui::PopID();
    }

    void RenderResetButtonFloat3(glm::vec3 &value, const glm::vec3 &default)
    {
        ImGui::PushID(&value);
        if (ImGui::Button(""))
            value = default;
        ImGui::PopID();
    }

    void RenderCheckBox(const std::string &name, bool &value)
    {
        ImGui::PushID(&value);
        ImGui::Text(name.c_str());
        ImGui::NextColumn();
        ImGui::Checkbox(name.c_str(), &value);
        ImGui::PopID();
    }

    // 声明：需在外部维护选中的Transform指针
    static inline TransformComponent *s_SelectedTransform = nullptr;

    // 递归渲染单个Transform节点及其子节点
    void RenderSceneNode(TransformComponent *transform)
    {
        // 生成唯一标识符（使用对象地址）
        ImGui::PushID(transform);

        // 树节点标志：根据是否有子节点设为叶节点
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
        if (transform->m_children.empty())
        {
            flags |= ImGuiTreeNodeFlags_Leaf;
        }

        // 是否被选中
        if (s_SelectedTransform == transform)
        {
            flags |= ImGuiTreeNodeFlags_Selected;
        }

        // 树节点名称（使用地址）
        // bool isOpen = ImGui::TreeNodeEx("##Node", flags, "Object_%p", transform);
        bool isOpen = ImGui::TreeNodeEx("##Node", flags, transform->gameObject->get_name().c_str());

        // 左键点击选中逻辑
        if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
        {
            s_SelectedTransform = transform;
        }

        // 右键菜单：附加/分离操作
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Attach Parent"))
            {
                if (s_SelectedTransform && s_SelectedTransform != transform)
                {
                    s_SelectedTransform->attach(transform);
                }
            }
            if (transform->m_father && ImGui::MenuItem("Detach"))
            {
                transform->detach();
            }
            ImGui::EndPopup();
        }

        if (isOpen)
        {
            // 递归渲染子节点
            for (auto *child : transform->m_children)
            {
                auto *childTransform = child->get_component<TransformComponent>();
                if (childTransform)
                {
                    RenderSceneNode(childTransform);
                }
            }

            ImGui::TreePop();
        }

        ImGui::PopID();
    }

    void RenderGameObjectInspector(TransformComponent *trans)
    {
        // 生成唯一标识符（使用对象地址）
        ImGui::PushID(trans);

        if (trans)
        {
            // 显示Transform信息
            ImGui::Indent();

            RenderDragFloat3("Position", trans->m_pos);
            ImGui::SameLine();
            RenderResetButtonFloat3(trans->m_pos, glm::vec3(0, 0, 0));

            RenderDragFloat3("Rotation", trans->m_rot, 1.0f, -360.0f, 360.0f);
            ImGui::SameLine();
            RenderResetButtonFloat3(trans->m_rot, glm::vec3(0, 0, 0));

            RenderDragFloat3("Scale", trans->m_scl, 0.1f, 0.00001);
            ImGui::SameLine();
            RenderResetButtonFloat3(trans->m_scl, glm::vec3(1, 1, 1));

            RenderCheckBox("Static", trans->m_static);
            ImGui::Unindent();
        }

        ImGui::PopID();
    }

    // 主场景树入口函数
    void RenderSceneTree(const std::vector<TransformComponent *> &rootTransforms)
    {
        ImGui::Begin("Scene Hierarchy");

        // 遍历所有根节点
        for (auto *root : rootTransforms)
        {
            if (root && root->m_father == nullptr)
            { // 确保是根节点
                RenderSceneNode(root);
            }
        }

        // 空白区域点击取消选择
        if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            s_SelectedTransform = nullptr;
        }

        ImGui::End();

        ImGui::Begin("Game Object Inspector");
        RenderGameObjectInspector(s_SelectedTransform);
        ImGui::End();
    }
}