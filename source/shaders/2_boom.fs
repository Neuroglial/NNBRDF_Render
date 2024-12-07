#version 420 core

out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D texture1;
uniform float lod;

void main()
{
    vec3 color = textureLod(texture1, TexCoords, lod).rgb;
    FragColor = vec4(color,1);
}