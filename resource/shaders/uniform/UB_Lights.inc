
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
#define MAX_POINT_LIGHTS_MAP 4

#define MAX_SPOT_LIGHTS 4
#define MAX_SPOT_LIGHTS_MAP 1

#define SHADOW_NEAR_PLANE 0.01f

uniform sampler2D DirLightMap[MAX_DIR_LIGHTS_MAP];
uniform samplerCube PtLightMap[MAX_POINT_LIGHTS_MAP+1];
uniform sampler2D SptLightMap[MAX_SPOT_LIGHTS_MAP];

struct DirectionalLight {
    float intensity;
    int dirMapIndex;
    
    vec3 direction;
    vec3 color;
};

struct PointLight {
    float intensity;
    float radius;
    int ptMapIndex;
    
    vec3 position;
    vec3 color;
};

struct SpotLight {
    float intensity;
    float cutOff;      // 内角余弦值（如 cos(12°)）
    float outerCutOff; // 外角余弦值（如 cos(15°)）
    float radius;
    int sptMapIndex;

    vec3 position;
    vec3 direction;
    vec3 color;
};

layout (std140,binding = 3) uniform UB_Lights_New
{
    vec3 lightNum;
    DirectionalLight dirLight[MAX_DIR_LIGHTS];
    PointLight ptLight[MAX_POINT_LIGHTS];
    SpotLight sptLight[MAX_SPOT_LIGHTS];
};

float GetPointLightFarPlane(PointLight light)
{
    return 100.0f;
}

float ShadowCalculation(PointLight light, vec3 fragPos)
{
    // Get vector between fragment position and light position
    vec3 fragToLight = fragPos - light.position;
    // Use the light to fragment vector to sample from the depth map    
    float closestDepth = texture(PtLightMap[light.ptMapIndex], fragToLight).r;
    // It is currently in linear range between [0,1]. Re-transform back to original value
    closestDepth *= GetPointLightFarPlane(light);
    // Now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    // Now test for shadows
    float bias = 0.05; 
    float shadow = currentDepth -  bias > closestDepth ? 0.0 : 1.0;

    return shadow;
}

// 计算点光源光照强度
void CalculatePointLight(PointLight light, vec3 fragPos, out vec3 lightDir, out vec3 lightColor)
{
    // 计算光源到片元的方向（L）
    lightDir = normalize(light.position - fragPos);
    
    // 计算点光源的衰减
    float distance = length(light.position - fragPos);
    float attenuation = light.intensity / (distance * distance);

    lightColor = light.color * attenuation * ShadowCalculation(light, fragPos);
}
