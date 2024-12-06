#version 420 core

out vec4 FragColor;

in vec2 TexCoord;
in vec3 Normal;

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
    FragColor = vec4(lg[0].light_color,1.0f);
}