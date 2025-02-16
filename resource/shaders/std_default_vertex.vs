#version 420 core
#include "resource/shaders/include/std_vs_in.inc"
#include "resource/shaders/uniform/UB_Camera.inc"

out vec2 texCoords;
out vec3 normal;
out vec3 fragPos;

uniform mat4 model;

void main()
{
    fragPos = vec3(model * vec4(Position, 1.0));
    normal = normalize(transpose(inverse(mat3(model)))*Normal);
    texCoords = TexCoords.xy;
    
    gl_Position = Projection * view * model * vec4(Position, 1.0f);
}