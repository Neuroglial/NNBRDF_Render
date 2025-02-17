#pragma once
#include <glm/glm.hpp>

inline const int UB_Base_Info_Binding = 0;

struct UB_Base_Info
{
    alignas(4) float iTime;
    alignas(4) float iTimeDelta;
    alignas(4) int iFrame;
    alignas(4) float iFrameRate;
    alignas(4) glm::vec4 iMouse;
    alignas(4) glm::vec3 iResolution;
};