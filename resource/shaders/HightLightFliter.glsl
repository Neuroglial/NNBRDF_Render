#vertex
#include "resource\shaders\b_post.vs"
#end

#fragment
#version 420 core

out vec4 fragColor;
in vec2 texCoords;

uniform sampler2D iChannel0;
const float threshold = 1.0f;

float brightness(vec3 color)
{
    return color.r * .3f + color.g * .59f + color.b * .11f;
}

void main() {
    vec3 color = texture(iChannel0,texCoords).rgb;
    fragColor = vec4(smoothstep(threshold,threshold+0.3f,brightness(color))*color,1.0f);
}
#end