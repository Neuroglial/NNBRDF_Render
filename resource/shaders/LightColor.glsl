#vertex
#include "resource\shaders\std_default_vertex.vs"
#end

#fragment
#version 420 core
#include "resource/shaders/uniform/UB_Lights.inc"
out vec4 fragColor;

in vec2 texCoords;
in vec3 normal;
in vec3 fragPos;

uniform vec3 lightColor;
uniform float intensity;
uniform vec3 colorAdd = vec3(1,0,0);

void main()
{    
    fragColor = vec4(lightColor * intensity * 2.0f + colorAdd, 1.0);
}
#end