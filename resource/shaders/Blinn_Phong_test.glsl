#vertex
#include "resource\shaders\std_default_vertex.vs"
#end



#fragment
#version 420 core

#include "resource/shaders/uniform/UB_Lights.inc"
#include "resource/shaders/uniform/UB_Camera.inc"

out vec4 fragColor;

in vec2 texCoords;
in vec3 normal;
in vec3 fragPos;

uniform sampler2D mt_diffuse;
uniform sampler2D mt_specular;
uniform samplerCube depthMap[1];
uniform float mt_shininess;

uniform vec3 lightPos;
uniform float far_plane;

// function prototypes
vec3 CalcDirLight(ST_DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(ST_PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(ST_SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

float ShadowCalculation()
{
    // Get vector between fragment position and light position
    vec3 fragToLight = fragPos - lightPos;
    // Use the light to fragment vector to sample from the depth map    
    float closestDepth = texture(depthMap[0], fragToLight).r;
    // It is currently in linear range between [0,1]. Re-transform back to original value
    closestDepth *= far_plane;
    // Now get current linear depth as the length between the fragment and light position
    float currentDepth = length(fragToLight);
    // Now test for shadows
    float bias = 0.05; 
    float shadow = currentDepth -  bias > closestDepth ? 0.0 : 1.0;

    return shadow;
}

void main()
{    
    // properties
    vec3 view_dir = normalize(ub_view_pos - fragPos);
    
    // == =====================================================
    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
    // For each phase, a calculate function is defined that calculates the corresponding color
    // per lamp. In the main() function we take all the calculated colors and sum them up for
    // this fragment's final color.
    // == =====================================================
    // phase 1: directional lighting
    vec3 result = vec3(0);
    for(int i=0; i< ub_dir_num;++i)
        result += CalcDirLight(ub_dir_lights[i], normal, view_dir);
    // phase 2: point lights
    for(int i = 0; i < ub_point_num; ++i)
        result += CalcPointLight(ub_point_lights[i], normal, fragPos, view_dir);    
    // phase 3: spot light
    for(int i = 0; i < ub_spot_num; ++i)
        result += CalcSpotLight(ub_spot_lights[i], normal, fragPos, view_dir);    
    
    fragColor = vec4(result, 1.0);
}

// calculates the color when using a directional light.
vec3 CalcDirLight(ST_DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), mt_shininess);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(mt_diffuse, texCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(mt_diffuse, texCoords)) * ShadowCalculation();
    vec3 specular = light.specular * spec * vec3(texture(mt_specular, texCoords))* ShadowCalculation();
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 CalcPointLight(ST_PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), mt_shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // combine results
    vec3 ambient = light.ambient * vec3(texture(mt_diffuse, texCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(mt_diffuse, texCoords)) * ShadowCalculation();
    vec3 specular = light.specular * spec * vec3(texture(mt_specular, texCoords)) * ShadowCalculation();
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

// calculates the color when using a spot light.
vec3 CalcSpotLight(ST_SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 halfwayDir = normalize(lightDir + viewDir);  
    float spec = pow(max(dot(normal, halfwayDir), 0.0), mt_shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(mt_diffuse, texCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(mt_diffuse, texCoords)) * ShadowCalculation();
    vec3 specular = light.specular * spec * vec3(texture(mt_specular, texCoords)) * ShadowCalculation();
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}

#end