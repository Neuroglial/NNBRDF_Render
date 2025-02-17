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

#define colorRange 0.25

vec3 jodieReinhardTonemap(vec3 c){
    float l = dot(c, vec3(0.2126, 0.7152, 0.0722));
    vec3 tc = c / (c + 1.0);

    return mix(c / (l + 1.0), tc, tc);
}

vec3 bloomTile(float lod, vec2 offset, vec2 uv){
    return texture(iChannel1, uv * exp2(-lod) + offset).rgb;
}

vec3 getBloom(vec2 uv){

    vec3 blur = vec3(0.0);

    blur = pow(bloomTile(2., vec2(0.0,0.0), uv),vec3(2.2))       	   	+ blur;
    blur = pow(bloomTile(3., vec2(0.3,0.0), uv),vec3(2.2)) * 1.3        + blur;
    blur = pow(bloomTile(4., vec2(0.0,0.3), uv),vec3(2.2)) * 1.6        + blur;
    blur = pow(bloomTile(5., vec2(0.1,0.3), uv),vec3(2.2)) * 1.9 	   	+ blur;
    blur = pow(bloomTile(6., vec2(0.2,0.3), uv),vec3(2.2)) * 2.2 	   	+ blur;

    return blur * colorRange;
}

void main()
{
    vec3 color = pow(texture(iChannel0, texCoords).rgb * colorRange, vec3(2.2));
    color = pow(color, vec3(2.2));
    color += pow(getBloom(texCoords), vec3(2.2));
    color = pow(color, vec3(1.0 / 2.2));
    color = jodieReinhardTonemap(color);
    
	fragColor = vec4(color,1.0);
}
#end