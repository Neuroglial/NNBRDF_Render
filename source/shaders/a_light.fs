#version 420 core

out vec4 fragColor;

in vec2 texCoord;
in vec3 normal;

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

layout (std140,binding = 2) uniform UB_Lightss
{
    int point_num;
    int dir_num;
    int spot_num;
    ST_PointLight point_lights[NR_POINT_LIGHTS_MAX];
    ST_DirLight dir_lights[NR_DIR_LIGHTS_MAX];
    ST_SpotLight spot_lights[NR_SPOT_LIGHTS_MAX];
};

void main()
{
    fragColor = vec4(point_lights[0].specular,1.0f);
}