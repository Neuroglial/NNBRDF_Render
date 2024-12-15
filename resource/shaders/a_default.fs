#version 420 core

struct ST_DirLight {
    vec3 direction;
	
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct ST_PointLight {
    float constant;
    float linear;
    float quadratic;
	
    vec3 position;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

struct ST_SpotLight {
    float cutOff;
    float outerCutOff;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 position;
    vec3 direction;
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;       
};

#define NR_POINT_LIGHTS_MAX     4
#define NR_DIR_LIGHTS_MAX       1
#define NR_SPOT_LIGHTS_MAX      1

layout (std140,binding = 2) uniform UB_Lights
{
    int point_num;
    int dir_num;
    int spot_num;
    ST_PointLight point_lights[NR_POINT_LIGHTS_MAX];
    ST_DirLight dir_lights[NR_DIR_LIGHTS_MAX];
    ST_SpotLight spot_lights[NR_SPOT_LIGHTS_MAX];
};

layout (std140,binding = 1) uniform UB_Camera
{
    vec3 view_pos;
    mat4 projection;
    mat4 view;
}; 

out vec4 fragColor;

in vec2 texCoords;
in vec3 normal;
in vec3 fragPos;

void main()
{
    fragColor = vec4(vec3(1.0), 1.0);
}