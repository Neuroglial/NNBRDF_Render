layout (std140,binding = 1) uniform UB_Camera
{
    vec3 ub_view_pos;
    mat4 ub_projection;
    mat4 ub_view;
}; 