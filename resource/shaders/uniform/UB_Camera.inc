layout (std140,binding = 1) uniform UB_Camera
{
    vec3 viewPos;
    mat4 Projection;
    mat4 view;
}; 