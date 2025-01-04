#version 420 core

out vec4 fragColor;

in vec2 texCoords;
in vec3 normal;
in vec3 fragPos;

void main()
{
    fragColor = vec4(vec3(1.0), 1.0);
}