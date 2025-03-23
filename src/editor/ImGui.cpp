#include "editor/ImGui.hpp"

namespace UI
{
	void PushID()
	{
		ImGui::PushID(s_UIContextID++);
		s_Counter = 0;
	}

	void PushID(void *ptr)
	{
		s_UIContextID++;
		ImGui::PushID(ptr);
		s_Counter = 0;
	}

	void PopID()
	{
		ImGui::PopID();
		s_UIContextID--;
	}

	void BeginPropertyGrid()
	{
		PushID();
		ImGui::Columns(2);
	}

	bool Property(const char *label, std::string &value, bool error)
	{
		bool modified = false;

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		char buffer[256];
		strcpy(buffer, value.c_str());

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		itoa(s_Counter++, s_IDBuffer + 2, 16);

		if (error)
			ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.9f, 0.2f, 0.2f, 1.0f));
		if (ImGui::InputText(s_IDBuffer, buffer, 256))
		{
			value = buffer;
			modified = true;
		}
		if (error)
			ImGui::PopStyleColor();
		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	void Property(const char *label, const char *value)
	{
		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		itoa(s_Counter++, s_IDBuffer + 2, 16);
		ImGui::InputText(s_IDBuffer, (char *)value, 256, ImGuiInputTextFlags_ReadOnly);

		ImGui::PopItemWidth();
		ImGui::NextColumn();
	}

	bool Property(const char *label, bool &value)
	{
		bool modified = false;

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		itoa(s_Counter++, s_IDBuffer + 2, 16);
		if (ImGui::Checkbox(s_IDBuffer, &value))
			modified = true;

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	bool Property(const char *label, int &value)
	{
		bool modified = false;

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		itoa(s_Counter++, s_IDBuffer + 2, 16);
		if (ImGui::DragInt(s_IDBuffer, &value))
			modified = true;

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	bool PropertySlider(const char *label, int &value, int min, int max)
	{
		bool modified = false;

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		itoa(s_Counter++, s_IDBuffer + 2, 16);
		if (ImGui::SliderInt(s_IDBuffer, &value, min, max))
			modified = true;

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	bool Property(const char *label, float &value, float delta, float min, float max)
	{
		bool modified = false;

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		itoa(s_Counter++, s_IDBuffer + 2, 16);
		if (ImGui::DragFloat(s_IDBuffer, &value, delta, min, max))
			modified = true;

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	bool Property(const char *label, glm::vec2 &value, float delta)
	{
		bool modified = false;

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		itoa(s_Counter++, s_IDBuffer + 2, 16);
		if (ImGui::DragFloat2(s_IDBuffer, glm::value_ptr(value), delta))
			modified = true;

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	bool PropertyColor(const char *label, glm::vec3 &value)
	{
		bool modified = false;

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		itoa(s_Counter++, s_IDBuffer + 2, 16);
		if (ImGui::ColorEdit3(s_IDBuffer, glm::value_ptr(value)))
			modified = true;

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	bool Property(const char *label, glm::vec3 &value, float delta)
	{
		bool modified = false;

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		itoa(s_Counter++, s_IDBuffer + 2, 16);
		if (ImGui::DragFloat3(s_IDBuffer, glm::value_ptr(value), delta))
			modified = true;

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	bool Property(const char *label, glm::vec4 &value, float delta)
	{
		bool modified = false;

		ImGui::Text(label);
		ImGui::NextColumn();
		ImGui::PushItemWidth(-1);

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		itoa(s_Counter++, s_IDBuffer + 2, 16);
		if (ImGui::DragFloat4(s_IDBuffer, glm::value_ptr(value), delta))
			modified = true;

		ImGui::PopItemWidth();
		ImGui::NextColumn();

		return modified;
	}

	bool Button(const char *label)
	{
		bool modified = false;

		ImGui::Text(label);
		ImGui::NextColumn();

		s_IDBuffer[0] = '#';
		s_IDBuffer[1] = '#';
		memset(s_IDBuffer + 2, 0, 14);
		itoa(s_Counter++, s_IDBuffer + 2, 16);
		if (ImGui::Button(s_IDBuffer, ImVec2(-1, 0)))
			modified = true;

		ImGui::NextColumn();

		return modified;
	}

	bool DrawVec3Control(const std::string &label, glm::vec3 &values, const float resetValue, const float min, const float max, const float columnWidth)
	{
		bool modified = false;

		ImGuiIO &io = ImGui::GetIO();
		auto boldFont = io.Fonts->Fonts[0];

		ImGui::PushID(label.c_str());

		ImGui::Columns(2);
		ImGui::SetColumnWidth(0, columnWidth);
		ImGui::Text(label.c_str());
		ImGui::NextColumn();

		ImGui::PushMultiItemsWidths(3, ImGui::CalcItemWidth());
		ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2{0, 0});

		float lineHeight = GImGui->Font->FontSize + GImGui->Style.FramePadding.y * 2.0f;
		ImVec2 buttonSize = {lineHeight + 3.0f, lineHeight};

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.9f, 0.2f, 0.2f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.8f, 0.1f, 0.15f, 1.0f});
		ImGui::PushFont(boldFont);
		if (ImGui::Button("X", buttonSize))
		{
			values.x = resetValue;
			modified = true;
		}

		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		modified |= ImGui::DragFloat("##X", &values.x, 0.1f, min, max, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.3f, 0.8f, 0.3f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.2f, 0.7f, 0.2f, 1.0f});
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Y", buttonSize))
		{
			values.y = resetValue;
			modified = true;
		}

		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		modified |= ImGui::DragFloat("##Y", &values.y, 0.1f, min, max, "%.2f");
		ImGui::PopItemWidth();
		ImGui::SameLine();

		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4{0.2f, 0.35f, 0.9f, 1.0f});
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4{0.1f, 0.25f, 0.8f, 1.0f});
		ImGui::PushFont(boldFont);
		if (ImGui::Button("Z", buttonSize))
		{
			values.z = resetValue;
			modified = true;
		}

		ImGui::PopFont();
		ImGui::PopStyleColor(3);

		ImGui::SameLine();
		modified |= ImGui::DragFloat("##Z", &values.z, 0.1f, min, max, "%.2f");
		ImGui::PopItemWidth();

		ImGui::PopStyleVar();

		ImGui::Columns(1);

		ImGui::PopID();

		return modified;
	}

	void EndPropertyGrid()
	{
		ImGui::Columns(1);
		PopID();
	}

	bool BeginTreeNode(const char *name, bool defaultOpen)
	{
		ImGuiTreeNodeFlags treeNodeFlags = ImGuiTreeNodeFlags_Framed | ImGuiTreeNodeFlags_SpanAvailWidth | ImGuiTreeNodeFlags_FramePadding;
		if (defaultOpen)
			treeNodeFlags |= ImGuiTreeNodeFlags_DefaultOpen;

		return ImGui::TreeNodeEx(name, treeNodeFlags);
	}

	void EndTreeNode()
	{
		ImGui::TreePop();
	}

	void DrawParams(ParamDic &params)
	{
		for (auto &i : params)
		{
			if (!i.second)
				continue;

			switch (i.second->type())
			{
			case ParamType::Int:
				if (auto *value = i.second->as<PM_Int>())
				{
					int val = value->changed() ? value->get() : 0;
					UI::Property(i.first.c_str(), val);
					*value = val;
				}
				break;

			case ParamType::Float:
				if (auto *value = i.second->as<PM_Float>())
				{
					float val = value->changed() ? value->get() : 0;
					UI::Property(i.first.c_str(), val);
					*value = val;
				}
				break;

			case ParamType::Vec2:
				if (auto *value = i.second->as<PM_Vec2>())
				{
					glm::vec2 val = value->changed() ? value->get() : glm::vec2(0, 0);
					UI::Property(i.first.c_str(), val);
					*value = val;
				}
				break;

			case ParamType::Vec3:
				if (auto *value = i.second->as<PM_Vec3>())
				{
					glm::vec3 val = value->changed() ? value->get() : glm::vec3(0, 0, 0);
					UI::PropertyColor(i.first.c_str(), val);
					*value = val;
				}
				break;

			case ParamType::Vec4:
				if (auto *value = i.second->as<PM_Vec4>())
				{
					glm::vec4 val = value->changed() ? value->get() : glm::vec4(0, 0, 0, 0);
					UI::Property(i.first.c_str(), val);
					*value = val;
				}
				break;

			default:
				break;
			}
		}
	}
}