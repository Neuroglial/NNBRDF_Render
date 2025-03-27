#pragma once

#include "scene/Component.hpp"
#include "scene/GameObject.hpp"

#include "editor/ImGui.hpp"
#include "editor/ComponentInspector.hpp"

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

    // Statement: The selected Transform pointer needs to be maintained externally
    static inline TransformComponent *s_SelectedTransform = nullptr;
    static inline bool rot_loc = true;

    // Recursively render a single Transform node and its children
    void RenderSceneNode(TransformComponent *transform)
    {
        // Generate a unique identifier (using the object address)
        ImGui::PushID(transform);

        // Tree node flag: set as leaf node based on whether there are child nodes
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
        if (transform->m_children.empty())
        {
            flags |= ImGuiTreeNodeFlags_Leaf;
        }

        // Is it selected?
        if (s_SelectedTransform == transform)
        {
            flags |= ImGuiTreeNodeFlags_Selected;
        }

        // Tree node name (using address)
        // bool isOpen = ImGui::TreeNodeEx("##Node", flags, "Object_%p", transform);
        bool isOpen = ImGui::TreeNodeEx("##Node", flags, transform->gameObject->get_name().c_str());

        // Left click on the logic
        if (ImGui::IsItemClicked(ImGuiMouseButton_Left))
        {
            s_SelectedTransform = transform;
        }

        // Right-click menu: Attach/detach operations
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
            // Recursively render child nodes
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
        // Generate a unique identifier (using the object address)
        ImGui::PushID(trans);

        if (trans)
        {
            // Display Transform Information
            ImGui::Indent();
            DrawInspector(trans->gameObject);
            ImGui::Unindent();
        }

        ImGui::PopID();
    }

    // Main scene tree entry function
    void RenderSceneTree(const std::vector<TransformComponent *> &rootTransforms)
    {
        ImGui::Begin("Scene Hierarchy");

        // Traverse all root nodes
        for (auto *root : rootTransforms)
        {
            if (root && root->m_father == nullptr)
            { // Make sure it is the root node
                RenderSceneNode(root);
            }
        }

        // Click in the blank area to deselect
        if (ImGui::IsWindowHovered() && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            s_SelectedTransform = nullptr;
        }

        ImGui::End();

        ImGui::Begin("Game Object Inspector");
        RenderGameObjectInspector(s_SelectedTransform);
        ImGui::End();
    }

    void SceneController(SceneManager& scene)
    {
        static std::string path = "resource/scene/test2.scene";
        ImGui::Begin("Scene");
        UI::Property("Scene_Path",path);
        if(ImGui::Button("Load"))
        {
            scene.loadScene(path);
        }
        ImGui::SameLine();
        if(ImGui::Button("Save"))
        {
            scene.saveScene(path);
        }
        ImGui::SameLine();
        if(ImGui::Button("Delete"))
        {
            scene.deleteScene();
        }
        ImGui::End();
    }
}