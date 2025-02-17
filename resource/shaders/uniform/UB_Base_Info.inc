layout (std140,binding = 0) uniform UB_Base_Info
{
    float   iTime;
    float   iTimeDelta;
    int     iFrame;
    float   iFrameRate;
    vec4    iMouse;
    vec3    iResolution;
};