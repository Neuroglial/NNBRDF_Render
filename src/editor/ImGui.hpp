#pragma once

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_internal.h"
#include "glm/glm.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "core/platform/renderAPI/Param.hpp"

namespace UI
{

	inline int s_UIContextID = 0;
	inline uint32_t s_Counter = 0;
	inline char s_IDBuffer[16];

	void PushID();
	void PushID(void *ptr);
	void PopID();
	void BeginPropertyGrid();
	bool Property(const char *label, std::string &value, bool error = false);
	void Property(const char *label, const char *value);
	bool Property(const char *label, bool &value);
	bool Property(const char *label, int &value);
	bool PropertySlider(const char *label, int &value, int min, int max);
	bool Property(const char *label, float &value, float delta = 0.1f, float min = 0.0f, float max = 0.0f);
	bool Property(const char *label, glm::vec2 &value, float delta = 0.1f);
	bool PropertyColor(const char *label, glm::vec3 &value);
	bool Property(const char *label, glm::vec3 &value, float delta = 0.1f);
	bool Property(const char *label, glm::vec4 &value, float delta = 0.1f);
	bool Button(const char *label);
	bool DrawVec3Control(const std::string &label, glm::vec3 &values, const float resetValue = 0.0f, const float min = 0.0f, const float max = 0.0f, const float columnWidth = 100.0f);
	void EndPropertyGrid();
	bool BeginTreeNode(const char *name, bool defaultOpen = true);
	void EndTreeNode();
	void DrawParams(ParamDic &params);
}