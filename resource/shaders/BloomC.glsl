#vertex
#include "resource\shaders\b_post.vs"
#end

#fragment
#version 420 core

#include "resource\shaders\uniform\UB_Base_Info.inc"

out vec4 fragColor;

in vec2 texCoords;

uniform sampler2D iChannel0;
uniform sampler2D iChannel1;


void main() {
    vec3 color = texture(iChannel0,texCoords).rgb;
    color += texture(iChannel1,texCoords).rgb;

    // HDR tonemapping
    color = color / (color + vec3(1.0));
    // gamma correctf
    color = pow(color, vec3(1.0/1.2f)); 

    fragColor = vec4(color,1.0f);
}
#end