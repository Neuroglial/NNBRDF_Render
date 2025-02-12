#pragma once

#include "scene/Component.hpp"
#include "scene/GameObject.hpp"

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"

namespace UI
{
    // 声明：需在外部维护选中的Transform指针
    static inline TransformComponent *s_SelectedTransform = nullptr;

    // 递归渲染单个Transform节点及其子节点
    void RenderTransformNode(TransformComponent *transform)
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

        // 点击选中逻辑
        if (ImGui::IsItemClicked())
        {
            s_SelectedTransform = transform;
        }

        // 同一行显示静态标记
        ImGui::SameLine();
        ImGui::Checkbox("##Static", &transform->m_static);

        // 右键菜单：附加/分离操作
        if (ImGui::BeginPopupContextItem())
        {
            if (ImGui::MenuItem("Attach Parent"))
            {
                // 实现附加父节点逻辑
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
                    RenderTransformNode(childTransform);
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
            ImGui::Text("Position: (%.2f, %.2f, %.2f)",
                        trans->m_pos.x, trans->m_pos.y, trans->m_pos.z);
            ImGui::Text("Rotation: (%.2f, %.2f, %.2f)",
                        trans->m_rot.x, trans->m_rot.y, trans->m_rot.z);
            ImGui::Text("Scale:    (%.2f, %.2f, %.2f)",
                        trans->m_scl.x, trans->m_scl.y, trans->m_scl.z);
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
                RenderTransformNode(root);
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