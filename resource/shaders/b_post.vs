#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

uniform mat4 model;
out vec2 texCoords;

void main()
{
    texCoords = aTexCoord;
    gl_Position = model * vec4(aPos, 1.0f);
}