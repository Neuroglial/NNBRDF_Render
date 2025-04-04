#vertex
#include "resource\shaders\std_default_vertex.vs"
#end

#fragment
#version 420 core
#include "resource/shaders/uniform/UB_Camera.inc"
out vec4 fragColor;
in vec2 texCoords;
in vec3 normal;
in vec3 fragPos;
uniform samplerCube iChannel0;

uniform float intensity;

void main()
{
    vec3 color = texture(iChannel0,fragPos-viewPos).rgb;
    fragColor = vec4(color * intensity, 1.0);
    //fragColor = vec4(vec3(texture(iChannel0,fragPos-viewPos).r), 1.0);
}
#end