// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#pragma once
#include <glm/glm.hpp>

// 所有UniformBuffer都在此处声明格式均为UB_XXXXXX

//---------------------------------------------------
struct UB_Base_Info
{
    float iTime;
    float iTimeDelta;
    int iFrame;
    float iFrameRate;
    alignas(16) glm::vec4 iMouse;
    alignas(16) glm::vec3 iResolution;
};
inline UB_Base_Info ub_base_info_data;
/*
layout (std140,binding = 0) uniform UB_Base_Info
{
    float   iTime;
    float   iTimeDelta;
    int     iFrame;
    float   iFrameRate;
    vec4    iMouse;
    vec3    iResolution;
};
*/

//---------------------------------------------------
struct UB_Camera
{
    glm::vec3 view_pos;
    alignas(16) glm::mat4 projection;
    alignas(16) glm::mat4 view;
};
inline UB_Camera ub_camera_data;
/*
layout(std140, binding = 1) uniform UB_Camera
{
    vec3 view_pos;
    mat4 projection;
    mat4 view;
};
*/

//---------------------------------------------------
struct ST_DirLight
{
    alignas(16) glm::vec3 direction;

    alignas(16) glm::vec3 ambient;
    alignas(16) glm::vec3 diffuse;
    alignas(16) glm::vec3 specular;
};

struct ST_PointLight
{
    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;
    float have_lightmap;

    alignas(16) glm::vec3 position;
    alignas(16) glm::vec3 ambient;
    alignas(16) glm::vec3 diffuse;
    alignas(16) glm::vec3 specular;
};

struct ST_SpotLight
{
    float cutOff = glm::cos(glm::radians(12.5f));
    float outerCutOff = glm::cos(glm::radians(15.0f));

    float constant = 1.0f;
    float linear = 0.09f;
    float quadratic = 0.032f;

    alignas(16) glm::vec3 direction;
    alignas(16) glm::vec3 position;
    alignas(16) glm::vec3 ambient;
    alignas(16) glm::vec3 diffuse;
    alignas(16) glm::vec3 specular;
};

#define NR_POINT_LIGHTS_MAX 4
#define NR_DIR_LIGHTS_MAX 1
#define NR_SPOT_LIGHTS_MAX 1

struct UB_Lights
{
    int point_num = 0;
    int dir_num = 0;
    int spot_num = 0;
    alignas(16) ST_PointLight point_lights[NR_POINT_LIGHTS_MAX];
    alignas(16) ST_DirLight dir_lights[NR_DIR_LIGHTS_MAX];
    alignas(16) ST_SpotLight spot_lights[NR_SPOT_LIGHTS_MAX];
};
inline UB_Lights ub_lights_data;
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
    int point_num;
    int dir_num;
    int spot_num;
    PointLight point_lights[NR_POINT_LIGHTS_MAX];
    DirLight dir_lights[NR_DIR_LIGHTS_MAX];
    SpotLight spot_lights[NR_SPOT_LIGHTS_MAX];
};
*/