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
    float have_lightmap;
	
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
    int             ub_point_num;
    int             ub_dir_num;
    int             ub_spot_num;
    ST_PointLight   ub_point_lights[NR_POINT_LIGHTS_MAX];
    ST_DirLight     ub_dir_lights[NR_DIR_LIGHTS_MAX];
    ST_SpotLight    ub_spot_lights[NR_SPOT_LIGHTS_MAX];
};