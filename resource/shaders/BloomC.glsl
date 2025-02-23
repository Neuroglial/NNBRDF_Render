#vertex
#include "resource\shaders\b_post.vs"
#end

#fragment
#version 420 core

#include "resource\shaders\uniform\UB_Base_Info.inc"
out vec4 fragColor;
in vec2 texCoords;
uniform sampler2D iChannel0;

// Horizontal blur

#define BLUR_SAMPLES 13
#define pi 3.141592
#define pow2(x) (x * x)

float gaussian(float x, float sigma) {
    return (1.0 / sqrt(2.0 * pi * pow2(sigma))) * exp(-(pow2(x) / (2.0 * pow2(sigma))));
}

vec4 blur(sampler2D sp, vec2 uv, vec2 scale) {
    vec4 col = vec4(0.0);
    float accum, weight, offset;
    
    for (int i = -BLUR_SAMPLES / 2; i < BLUR_SAMPLES / 2; ++i) {
        offset = float(i);
        weight = gaussian(offset, sqrt(float(BLUR_SAMPLES)));
        col += texture(sp, uv + scale * offset) * weight;
        accum += weight;
    }
    
    return col / accum;
}

void main() {
    vec2 ps = vec2(1.0) / iResolution.xy;
    
    vec4 color = blur(iChannel0, texCoords, vec2(0.0, ps.y));
    fragColor = vec4(color.rgb,1.0f);
}

#end