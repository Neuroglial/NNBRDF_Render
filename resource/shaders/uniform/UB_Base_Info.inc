layout (std140,binding = 0) uniform UB_Base_Info
{
    float   ub_iTime;
    float   ub_iTimeDelta;
    int     ub_iFrame;
    float   ub_iFrameRate;
    vec4    ub_iMouse;
};