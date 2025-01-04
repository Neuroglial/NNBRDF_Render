#version 420 core

#include "resource/shaders/uniform/UB_Lights.inc"

out vec4 fragColor;

in vec2 texCoord;
in vec3 normal;

void main()
{
    fragColor = vec4(ub_point_lights[0].specular,1.0f);
}