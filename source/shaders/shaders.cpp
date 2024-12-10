﻿#include "scene/ShaderManager.hpp"
#include "core/platform/renderAPI/RenderAPI.hpp"
#include "core/platform/renderAPI/ShaderParam.hpp"

/*
    在此处声明shader,格式如下:
    class a_default_vs  //以 文件名_vs(fs) 命名方便寻找
    {
    public:
        BEGIN_SHADER_PARAM_STRUCT()  //定义参数 SD为前缀为支持的参数类型
            SHADER_PARAM(SDMat4, model)  
        END_SHADER_PARAM_STRUCT()

        DECLARE_SHADER(a_default_vs)
    };
    REGISTER_SHADER(a_default_vs, Root_Path + "source/shaders/a_default.vs", Shader_Type::VERTEX_SHADER)
*/



class a_default_vs
{
public:
    BEGIN_SHADER_PARAM_STRUCT()
    SHADER_PARAM(SDMat4, model)
    END_SHADER_PARAM_STRUCT()

    DECLARE_SHADER(a_default_vs)
};
REGISTER_SHADER(a_default_vs, Root_Path + "source/shaders/a_default.vs", Shader_Type::VERTEX_SHADER)

class a_default_fs
{
public:
    BEGIN_SHADER_PARAM_STRUCT()
    SHADER_PARAM(SDTexture2D, texture1)
    SHADER_PARAM(SDTexture2D, texture2)
    END_SHADER_PARAM_STRUCT()

    DECLARE_SHADER(a_default_fs)
};
REGISTER_SHADER(a_default_fs, Root_Path + "source/shaders/a_default.fs", Shader_Type::FRAGMENT_SHADER)

class a_light_fs
{
public:
    BEGIN_SHADER_PARAM_STRUCT()
    END_SHADER_PARAM_STRUCT()

    DECLARE_SHADER(a_light_fs)
};
REGISTER_SHADER(a_light_fs, Root_Path + "source/shaders/a_light.fs", Shader_Type::FRAGMENT_SHADER)

class b_post_vs
{
public:
    BEGIN_SHADER_PARAM_STRUCT()
    END_SHADER_PARAM_STRUCT()

    DECLARE_SHADER(b_post_vs)
};
REGISTER_SHADER(b_post_vs, Root_Path + "source/shaders/b_post.vs", Shader_Type::VERTEX_SHADER)

class b_luminance_fs
{
public:
    BEGIN_SHADER_PARAM_STRUCT()
    SHADER_PARAM(SDTexture2D, texture1)
    END_SHADER_PARAM_STRUCT()

    DECLARE_SHADER(b_luminance_fs)
};
REGISTER_SHADER(b_luminance_fs, Root_Path + "source/shaders/b_luminance.fs", Shader_Type::FRAGMENT_SHADER)

class b_boom_fs
{
public:
    BEGIN_SHADER_PARAM_STRUCT()
    SHADER_PARAM(SDTexture2D, texture1)
    SHADER_PARAM(SDFloat, lod)
    END_SHADER_PARAM_STRUCT()

    DECLARE_SHADER(b_boom_fs)
};
REGISTER_SHADER(b_boom_fs, Root_Path + "source/shaders/b_boom.fs", Shader_Type::FRAGMENT_SHADER)