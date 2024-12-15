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

uniform sampler2D mt_diffuse;
uniform sampler2D mt_specular;
uniform float mt_shininess;

// function prototypes
vec3 CalcDirLight(ST_DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(ST_PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(ST_SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{    
    // properties
    vec3 view_dir = normalize(view_pos - fragPos);
    
    // == =====================================================
    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
    // For each phase, a calculate function is defined that calculates the corresponding color
    // per lamp. In the main() function we take all the calculated colors and sum them up for
    // this fragment's final color.
    // == =====================================================
    // phase 1: directional lighting
    vec3 result = vec3(0);
    for(int i=0; i< dir_num;++i)
        result += CalcDirLight(dir_lights[i], normal, view_dir);
    // phase 2: point lights
    for(int i = 0; i < point_num; ++i)
        result += CalcPointLight(point_lights[i], normal, fragPos, view_dir);    
    // phase 3: spot light
    for(int i = 0; i < spot_num; ++i)
        result += CalcSpotLight(spot_lights[i], normal, fragPos, view_dir);    
    
    fragColor = vec4(result, 1.0);
}

// calculates the color when using a directional light.
vec3 CalcDirLight(ST_DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mt_shininess);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(mt_diffuse, texCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(mt_diffuse, texCoords));
    vec3 specular = light.specular * spec * vec3(texture(mt_specular, texCoords));
    return (ambient + diffuse + specular);
}

// calculates the color when using a point light.
vec3 CalcPointLight(ST_PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragPos);
    // diffuse shading
    float diff = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mt_shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));
    // combine results
    vec3 ambient = light.ambient * vec3(texture(mt_diffuse, texCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(mt_diffuse, texCoords));
    vec3 specular = light.specular * spec * vec3(texture(mt_specular, texCoords));
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
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), mt_shininess);
    // attenuation
    float distance = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
    // spotlight intensity
    float theta = dot(lightDir, normalize(-light.direction)); 
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((theta - light.outerCutOff) / epsilon, 0.0, 1.0);
    // combine results
    vec3 ambient = light.ambient * vec3(texture(mt_diffuse, texCoords));
    vec3 diffuse = light.diffuse * diff * vec3(texture(mt_diffuse, texCoords));
    vec3 specular = light.specular * spec * vec3(texture(mt_specular, texCoords));
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return (ambient + diffuse + specular);
}