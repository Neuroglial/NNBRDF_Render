// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#pragma once
#include <glm/glm.hpp>
#include "resource/shaders/uniform/UB_Camera.hpp"

// 所有UniformBuffer都在此处声明格式均为UB_XXXXXX

//---------------------------------------------------
/*
layout (std140,binding = 0) uniform UB_Base_Info
{
    float   iTime;
    float   iTimeDelta
    int     iFrame;
    float   iFrameRate;
    vec4    iMouse;
};
*/

//---------------------------------------------------
/*
layout(std140, binding = 1) uniform UB_Camera
{
    vec3 viewPos;
    mat4 Projection;
    mat4 view;
};
*/

//---------------------------------------------------
/*
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
*/