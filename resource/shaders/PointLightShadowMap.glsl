#vertex
#version 420 core
#include "resource/shaders/include/std_vs_in.inc"
uniform mat4 model;
void main()
{
    gl_Position = model * vec4(Position, 1.0);
}
#end

#geometry
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
#end

#fragment
#version 420 core
in vec4 FragPos;

uniform vec3 lightPos;
uniform float far_plane;

void main()
{
    // get distance between fragment and light source
    float lightDistance = length(FragPos.xyz - lightPos);

    // map to [0;1] range by dividing by far_plane
    lightDistance = lightDistance / far_plane;

    // write this as modified depth
    gl_FragDepth = lightDistance;
}
#end