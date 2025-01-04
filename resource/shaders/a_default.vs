#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

#include "resource/shaders/uniform/UB_Camera.inc"

out vec2 texCoords;
out vec3 normal;
out vec3 fragPos;

uniform mat4 model;

void main()
{
    fragPos = vec3(model * vec4(aPos, 1.0));
    normal = normalize(transpose(inverse(mat3(model)))*aNormal);
    texCoords = vec2(aTexCoord.x, aTexCoord.y);
    
    gl_Position = ub_projection * ub_view * model * vec4(aPos, 1.0f);
}

/*
#version 420 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

void main()
{
    FragColor = vec4(1.0);
}
*/