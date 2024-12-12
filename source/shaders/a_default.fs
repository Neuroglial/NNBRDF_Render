#version 420 core

out vec4 fragColor;

in vec2 texCoords;
in vec3 normal;
in vec3 fragPos;

uniform sampler2D texture1;
uniform sampler2D texture2;


layout (std140,binding = 1) uniform UB_Camera
{
    vec3 view_pos;
    mat4 projection;
    mat4 view;
}; 

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
    vec3 objectColor = mix(texture(texture1, texCoords).rgb, texture(texture2, texCoords).rgb, 0.2);

    vec3 result = vec3(0);

    for(int i=0;i<pl_num;++i){
        // ambient
        float ambientStrength = 0.1;
        vec3 ambient = ambientStrength * pl[i].color;
    
        // diffuse 
        vec3 norm = normalize(normal);
        vec3 lightDir = normalize(pl[i].position - fragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * pl[i].color;

        // specular
        float specularStrength = 0.5;
        vec3 viewDir = normalize(view_pos - fragPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = specularStrength * spec * pl[i].color;  

        result += (ambient + diffuse + specular) * objectColor;
    }

    
    fragColor = vec4(result, 1.0);
}