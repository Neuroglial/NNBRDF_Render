#version 420 core
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 shadowMat_0;
uniform mat4 shadowMat_1;
uniform mat4 shadowMat_2;
uniform mat4 shadowMat_3;
uniform mat4 shadowMat_4;
uniform mat4 shadowMat_5;

out vec4 FragPos; // FragPos from GS (output per emitvertex)

void main()
{
    gl_Layer = 0;
    for(int i = 0; i < 3; ++i)
    {
        FragPos = gl_in[i].gl_Position;
        gl_Position = shadowMat_0 * FragPos;
        EmitVertex();
    }    
    EndPrimitive();

    gl_Layer = 1;
    for(int i = 0; i < 3; ++i)
    {
        FragPos = gl_in[i].gl_Position;
        gl_Position = shadowMat_1 * FragPos;
        EmitVertex();
    }    
    EndPrimitive();

    gl_Layer = 2;
    for(int i = 0; i < 3; ++i)
    {
        FragPos = gl_in[i].gl_Position;
        gl_Position = shadowMat_2 * FragPos;
        EmitVertex();
    }    
    EndPrimitive();

    gl_Layer = 3;
    for(int i = 0; i < 3; ++i)
    {
        FragPos = gl_in[i].gl_Position;
        gl_Position = shadowMat_3 * FragPos;
        EmitVertex();
    }    
    EndPrimitive();

    gl_Layer = 4;
    for(int i = 0; i < 3; ++i)
    {
        FragPos = gl_in[i].gl_Position;
        gl_Position = shadowMat_4 * FragPos;
        EmitVertex();
    }    
    EndPrimitive();

    gl_Layer = 5;
    for(int i = 0; i < 3; ++i)
    {
        FragPos = gl_in[i].gl_Position;
        gl_Position = shadowMat_5 * FragPos;
        EmitVertex();
    }    
    EndPrimitive();
    
}