#pragma once
#include <glm/glm.hpp>

inline const int UB_Camera_Binding = 1;

struct UB_Camera
{
    glm::vec3 viewPos;
    alignas(16) glm::mat4 projection;
    alignas(16) glm::mat4 view;
};