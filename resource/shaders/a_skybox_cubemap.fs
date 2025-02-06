#version 420 core

#include "resource/shaders/uniform/UB_Camera.inc"

out vec4 fragColor;

in vec2 texCoords;
in vec3 normal;
in vec3 fragPos;

uniform samplerCube iChannel0;

void main()
{
    vec3 color = texture(iChannel0,fragPos-ub_view_pos).rgb;
    fragColor = vec4(color, 1.0);
    //fragColor = vec4(vec3(texture(iChannel0,fragPos-ub_view_pos).r), 1.0);
}