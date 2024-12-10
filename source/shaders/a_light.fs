#version 420 core

out vec4 fragColor;

in vec2 texCoord;
in vec3 normal;

struct light{
    vec3 light_pos;
    vec3 light_color;
};

layout (std140,binding = 1) uniform Light
{
    int num;
    light lg[10];
};

uniform vec3 lightColor;

void main()
{
    fragColor = vec4(lg[0].light_color,1.0f);
}