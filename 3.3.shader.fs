#version 330 core

uniform vec3 color2;

out vec4 FragColor;  
in vec3 ourColor;

void main()
{
    FragColor = vec4(ourColor+color2, 1.0);
}