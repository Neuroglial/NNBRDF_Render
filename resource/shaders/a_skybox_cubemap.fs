#version 420 core

layout (std140,binding = 1) uniform UB_Camera
{
    vec3 view_pos;
    mat4 projection;
    mat4 view;
}; 

out vec4 fragColor;

in vec2 texCoords;
in vec3 normal;
in vec3 fragPos;

uniform samplerCube iChannel0;

void main()
{
    vec3 color = texture(iChannel0,fragPos-view_pos).rgb;
    fragColor = vec4(color, 1.0);
}