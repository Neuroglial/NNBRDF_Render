#define MAX_DIR_LIGHTS 4
#define MAX_DIR_LIGHTS_MAP 1

#define MAX_POINT_LIGHTS 4
#define MAX_POINT_LIGHTS_MAP 4

#define MAX_SPOT_LIGHTS 4
#define MAX_SPOT_LIGHTS_MAP 1

#define SHADOW_NEAR_PLANE 0.01f

uniform sampler2D DirLightMap[MAX_DIR_LIGHTS_MAP + 1];
uniform samplerCube PtLightMap[MAX_POINT_LIGHTS_MAP + 1];
uniform sampler2D SptLightMap[MAX_SPOT_LIGHTS_MAP + 1];

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
    float cutOff;
    float outerCutOff;
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

// Calculate the illumination intensity of a point light source
void CalculatePointLight(PointLight light, vec3 fragPos, out vec3 lightDir, out vec3 radiance)
{
    // Calculate the direction from the light source to the chip element (L)
    lightDir = normalize(light.position - fragPos);
    
    // Calculate the attenuation of point light sources
    float distance = length(light.position - fragPos);
    float attenuation = light.intensity / (distance * distance);

    radiance = light.color * attenuation * ShadowCalculation(light, fragPos);
}
