#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

layout (std140,binding = 2) uniform Base
{
    float   iTime;
    float   iTimeDelta;
    int     iFrame;
    float   iFrameRate;
    vec4    iMouse;
    vec3    iResolution;
};

out vec2 texCoords;

void main()
{
    texCoords = aTexCoord;
    gl_Position = vec4(aPos,1);
}