#version 420 core

out vec4 FragColor;
in vec2 TexCoords;
uniform sampler2D texture1;


void main()
{
    vec3 color = texture(texture1, TexCoords).rgb;
    color = 0.01+color;
    float p = 2;
    color = vec3(pow(color.x,p),pow(color.y,p),pow(color.z,p));
    FragColor = vec4(color,1);
}