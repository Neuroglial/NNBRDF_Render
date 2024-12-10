#version 420 core

out vec4 fragColor;
in vec2 texCoords;
uniform sampler2D texture1;
uniform float lod;

void main()
{
    vec3 color = textureLod(texture1, texCoords, lod).rgb;
    fragColor = vec4(color,1);
}