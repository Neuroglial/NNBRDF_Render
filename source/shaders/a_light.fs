#version 420 core

out vec4 fragColor;

in vec2 texCoord;
in vec3 normal;

struct PointLight {
    float intensity;        // 光源强度
    float attConstant;      // 常数衰减
    float attLinear;        // 线性衰减
    float attQuadratic;     // 平方衰减
    vec3 position;          // 光源位置
    vec3 color;             // 光源颜色
};

layout (std140,binding = 2) uniform UB_Light
{
    int pl_num;
    PointLight pl[10];
};

void main()
{
    fragColor = vec4(pl[0].color,1.0f);
}