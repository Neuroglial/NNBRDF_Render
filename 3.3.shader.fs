#version 330 core

uniform vec3 myColor;

out vec4 FragColor;  
in vec3 ourColor;

void main()
{
    FragColor = vec4(myColor, 1.0);
}