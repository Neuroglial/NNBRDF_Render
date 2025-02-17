#vertex
#include "resource\shaders\std_default_vertex.vs"
#end

#fragment
#version 420 core

out vec4 fragColor;

in vec2 texCoords;
in vec3 normal;
in vec3 fragPos;

uniform vec3 lightColor;

void main()
{     
    
    fragColor = vec4(lightColor, 1.0);
}
#end