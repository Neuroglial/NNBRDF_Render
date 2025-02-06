
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


//new light data struct

#define MAX_DIR_LIGHTS 4
#define MAX_DIR_LIGHTS_MAP 1

#define MAX_POINT_LIGHTS 4
#define MAX_POINT_LIGHTS_MAP 1

#define MAX_SPOT_LIGHTS 4
#define MAX_SPOT_LIGHTS_MAP 1

uniform sampler2D DirLightMap[MAX_DIR_LIGHTS_MAP];
uniform samplerCube PtLightMap[MAX_POINT_LIGHTS_MAP];
uniform sampler2D SptLightMap[MAX_SPOT_LIGHTS_MAP];

struct DirectionalLight {
    vec3 direction;
    vec3 color;
    float intensity;

    int dirMapIndex;
};

struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
    float radius;

    int ptMapIndex;
};

struct SpotLight {
    vec3 position;
    vec3 direction;
    vec3 color;
    float intensity;
    float cutOff;      // 内角余弦值（如 cos(12°)）
    float outerCutOff; // 外角余弦值（如 cos(15°)）
    float radius;

    int sptMapIndex;
};

layout (std140,binding = 3) uniform UB_Lights_New
{
    vec3 lightNum;
    DirectionalLight dirLight[MAX_DIR_LIGHTS];
    PointLight ptLight[MAX_POINT_LIGHTS];
    SpotLight sptLight[MAX_SPOT_LIGHTS];
};