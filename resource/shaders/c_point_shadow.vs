#version 420 core
#include "resource/shaders/include/std_vs_in.inc"
uniform mat4 model;
void main()
{
    gl_Position = model * vec4(Position, 1.0);
}