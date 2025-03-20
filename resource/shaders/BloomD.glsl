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


// Final blend

uniform float Bloom_Intensity = 0.5;
#define BLOOM_STEPS 9

uniform float gamma;

const vec2[] offsets = vec2[](
    vec2(0.05, 0.075),
    vec2(0.975, 0.05),
    vec2(0.975, 0.55),
    vec2(0.975, 0.875),
    vec2(0.7, 0.95),
    vec2(0.65, 0.7),
    vec2(0.65, 0.5),
    vec2(0.475, 0.65),
    vec2(0.325, 0.65)
);

vec3 saturate(vec3 x)
{
    return clamp(x, vec3(0.0), vec3(1.0));
}

vec2 scale_uv(vec2 uv, vec2 scale, vec2 center) {
    return (uv - center) * scale + center;
}

vec4 read_bloom(sampler2D sp, vec2 uv, float lod, vec2 center) {
    uv = scale_uv(uv, vec2(1.0 / pow(2.0, lod)), center);
    return texture(sp, uv);
}

vec4 get_bloom(sampler2D sp, vec2 uv, int steps) {
    vec4 col = vec4(0.0);
    
    for (int i = 1; i <= steps; ++i)
        col += read_bloom(sp, uv, float(i), offsets[i - 1]);
    
    return col / float(steps);
}

vec3 screen(vec3 a, vec3 b, float w) {
    return mix(a, vec3(1.0) - (vec3(1.0) - a) * (vec3(1.0) - b), w);
}

vec3 tonemap(vec3 col) {
    return col / (vec3(1.0) + col);
}

void main() {

    vec3 color;
    vec3 bloom = get_bloom(iChannel1, texCoords, BLOOM_STEPS).rgb;
    color = texture(iChannel0, texCoords).rgb;
    color += bloom * Bloom_Intensity;
    color = tonemap(color);
    color *= 1.0 - smoothstep(0.0, 2.0, distance(texCoords, vec2(0.5)));
    
    //Tonemapping and color grading
    color = color / (1.0 + color);
	color = saturate(color * 1.01);
    
    color = pow(color, vec3(gamma));
    fragColor = vec4(color, 1.0);
}

#end