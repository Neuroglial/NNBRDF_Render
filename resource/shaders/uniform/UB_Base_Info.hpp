#pragma once
#include <glm/glm.hpp>

inline const int UB_Base_Info_Binding = 0;

struct UB_Base_Info
{
    alignas(4) float iTime = 1.0f;
    alignas(4) float iTimeDelta = 0.01f;
    alignas(4) int iFrame = 1;
    alignas(4) float iFrameRate = 100;
    alignas(4) glm::vec4 iMouse = glm::vec4(0);
    alignas(4) glm::vec3 iResolution = glm::vec3(1080, 1920, 1);
};