#version 420 core

out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;

uniform sampler2D texture1;
uniform sampler2D texture2;

struct light{
    vec3 light_pos;
    vec3 light_color;
};

struct PointLight {
    vec3 position;          // 光源位置
    vec3 color;             // 光源颜色
    float intensity;        // 光源强度
    float attConstant;      // 常数衰减
    float attLinear;        // 线性衰减
    float attQuadratic;     // 平方衰减
};

layout (std140,binding = 0) uniform Camera
{
    vec3 view_pos;
}; 

layout (std140,binding = 1) uniform Light
{
    int num;
    light lg[10];
}; 

void main()
{
    vec3 objectColor = mix(texture(texture1, TexCoords).rgb, texture(texture2, TexCoords).rgb, 0.2);

    vec3 result = vec3(0);

    for(int i=0;i<num;++i){
        // ambient
        float ambientStrength = 0.1;
        vec3 ambient = ambientStrength * lg[i].light_color;
    
        // diffuse 
        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(lg[i].light_pos - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = diff * lg[i].light_color;

        // specular
        float specularStrength = 0.5;
        vec3 viewDir = normalize(view_pos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
        vec3 specular = specularStrength * spec * lg[i].light_color;  

        result += (ambient + diffuse + specular) * objectColor;
    }

    
    FragColor = vec4(result, 1.0);
}