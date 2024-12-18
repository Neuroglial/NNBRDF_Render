#version 420 core

layout (std140,binding = 1) uniform UB_Camera
{
    vec3 view_pos;
    mat4 projection;
    mat4 view;
}; 

out vec4 fragColor;

in vec2 texCoords;
in vec3 normal;
in vec3 fragPos;

uniform sampler2D iChannel0;

const vec2 invAtan = vec2(0.1591, 0.3183);
vec2 SampleSphericalMap(vec3 v)
{
    vec2 uv = vec2(atan(v.z, v.x), asin(v.y));
    uv *= invAtan;
    uv += 0.5;
    return uv;
}

void main()
{
    vec3 color = texture(iChannel0,SampleSphericalMap(normalize(fragPos-view_pos))).rgb;
    fragColor = vec4(color, 1.0);
}