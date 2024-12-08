#version 420 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoord;

out vec2 TexCoords;
out vec3 Normal;
out vec3 FragPos;

layout (std140,binding = 0) uniform Camera
{
    vec3 view_pos;
    mat4 projection;
    mat4 view;
}; 

uniform mat4 model;

void main()
{
    FragPos = vec3(model * vec4(aPos, 1.0));
    Normal = transpose(inverse(mat3(model)))*aNormal;
    TexCoords = vec2(aTexCoord.x, aTexCoord.y);
    
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
}

/*
#version 420 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

void main()
{
    FragColor = vec4(1.0);
}
*/