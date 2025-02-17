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
uniform float mt_shininess;

vec3 CalcBlinnPhong(vec3 lightColor, vec3 normal, vec3 diff, vec3 spec, float shininess, vec3 lightDir, vec3 viewDir)
{
     vec3 halfDir = normalize(viewDir + lightDir);
     diff =  (max(dot(normal, lightDir), 0.0) + 0.1f) * diff;  // diff + ambient
     spec = pow(max(dot(normal, halfDir), 0.0), shininess) * spec; // spec
     return (diff + spec) * lightColor;
}


void main()
{    
    // properties
    vec3 lightDir = vec3(0);
    vec3 lightColor = vec3(0);

    vec3 viewDir = normalize(viewPos - fragPos).rgb;
    vec3 diff = texture(mt_diffuse,texCoords).rgb;
    vec3 spec = texture(mt_specular,texCoords).rgb;
    
    // == =====================================================
    // Our lighting is set up in 3 phases: directional, point lights and an optional flashlight
    // For each phase, a calculate function is defined that calculates the corresponding color
    // per lamp. In the main() function we take all the calculated colors and sum them up for
    // this fragment's final color.
    // == =====================================================
    // phase 1: directional lighting
    vec3 result = vec3(0);

    for(int i = 0; i < lightNum.y; ++i)
    {
        CalculatePointLight(ptLight[i], fragPos, lightDir, lightColor);
        result += CalcBlinnPhong(lightColor,normal,diff,spec,mt_shininess,lightDir,viewDir);
    } 
    
    fragColor = vec4(result, 1.0);
}
#end