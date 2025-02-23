#vertex
#include "resource\shaders\b_post.vs"
#end

#fragment
#version 420 core
#include "resource\shaders\uniform\UB_Base_Info.inc"
out vec4 fragColor;
in vec2 texCoords;
uniform sampler2D iChannel0;

// Split source into lods
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

vec2 scale_uv(vec2 uv, vec2 scale, vec2 center) {
    return (uv - center) * scale + center;
}

void draw(inout vec4 color, sampler2D sp, vec2 uv, float lod, vec2 center) {
    uv = scale_uv(uv, vec2(pow(2.0, lod)), center);
    
    if (uv.x >= -0.1 && uv.x <= 1.1 && uv.y >= -0.1 && uv.y <= 1.1)
        color += textureLod(sp, uv, lod);
}

void main() {
    vec4 color = vec4(0.0);
    
    for (int i = 1; i < 10; ++i)
        draw(color, iChannel0, texCoords, float(i), offsets[i - 1]);

    fragColor = vec4(color.rgb,1.0f);
}

#end