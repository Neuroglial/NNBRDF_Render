#include <glm/glm.hpp>

//所有UniformBuffer都在此处声明格式均为UB_XXXXXX

//---------------------------------------------------
struct UB_Base_Info
{
    float iTime;
    float iTimeDelta;
    int iFrame;
    float iFrameRate;
    alignas(16) glm::vec4 iMouse;
    alignas(16) glm::vec3 iResolution;
};
inline UB_Base_Info ub_base_info_data;
/*
layout (std140,binding = 0) uniform UB_Base_Info
{
    float   iTime;
    float   iTimeDelta;
    int     iFrame;
    float   iFrameRate;
    vec4    iMouse;
    vec3    iResolution;
};
*/

//---------------------------------------------------
struct UB_Camera
{
    glm::vec3 view_pos;
    alignas(16) glm::mat4 projection;
    alignas(16) glm::mat4 view;
};
inline UB_Camera ub_camera_data;
/*
layout(std140, binding = 1) uniform UB_Camera
{
    vec3 view_pos;
    mat4 projection;
    mat4 view;
};
*/

//---------------------------------------------------
struct PointLight
{
    float intensity;                // 光源强度
    float attConstant;              // 常数衰减
    float attLinear;                // 线性衰减
    float attQuadratic;             // 平方衰减
    alignas(16) glm::vec3 position; // 光源位置
    alignas(16) glm::vec3 color;    // 光源颜色
};

struct UB_Light
{
    int pl_num;
    alignas(16) PointLight pl[10];
};
inline UB_Light ub_light_data;
/*
struct PointLight {
    float intensity;        // 光源强度
    float attConstant;      // 常数衰减
    float attLinear;        // 线性衰减
    float attQuadratic;     // 平方衰减
    vec3 position;          // 光源位置
    vec3 color;             // 光源颜色
};

layout (std140,binding = 2) uniform Light
{
    int pl_num;
    PointLight pl[10];
};
*/