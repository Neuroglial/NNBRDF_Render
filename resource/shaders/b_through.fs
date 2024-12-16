#version 420 core

out vec4 fragColor;
in vec2 texCoords;
uniform sampler2D iChannel0;

void main()
{
    vec3 color = texture(iChannel0, texCoords).rgb;
    fragColor = vec4(color,1);
}