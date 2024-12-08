#pragma once

#include "core/platform/renderAPI/Shader.hpp"
#include "core/platform/renderAPI/OpenGL/Shader_GL.hpp"
#include "scene/ShaderManager.hpp"
#include "utils/utils.hpp"

#ifdef __SHADER_MANAGER__
#define IMPLEMENT_SHADER(ClassName, Path, ShaderType) ClassName ClassName::instance(Path, ShaderType);
#else
#define IMPLEMENT_SHADER(ClassName, Path, ShaderType)
#endif

/*
    在此处声明shader,格式如下:
    class FragmentShader:public Shader_GL{
    public:

    BEGIN_SHADER_PARAM_STRUCT()
        SHADER_PARAM(SDTexture2D,texture1)
        SHADER_PARAM(SDTexture2D,texture2)
        SHADER_PARAM(SDVec3,color1)
    END_SHADER_PARAM_STRUCT()

    DECLARE_SHADER(FragmentShader)
    };
    IMPLEMENT_SHADER(FragmentShader,"../fragment_shader.glsl",Shader_Type::FRAGMENT_SHADER)
*/

class DefaultVS_1 : public Shader_GL
{
public:
    BEGIN_SHADER_PARAM_STRUCT()
    SHADER_PARAM(SDMat4, model)
    END_SHADER_PARAM_STRUCT()

    DECLARE_SHADER(DefaultVS_1)
};
IMPLEMENT_SHADER(DefaultVS_1, Root_Path + "source/shaders/1_default.vs", Shader_Type::VERTEX_SHADER)

class DefaultFS_1 : public Shader_GL
{
public:
    BEGIN_SHADER_PARAM_STRUCT()
    SHADER_PARAM(SDTexture2D, texture1)
    SHADER_PARAM(SDTexture2D, texture2)
    END_SHADER_PARAM_STRUCT()

    DECLARE_SHADER(DefaultFS_1)
};
IMPLEMENT_SHADER(DefaultFS_1, Root_Path + "source/shaders/1_default.fs", Shader_Type::FRAGMENT_SHADER)

class LightFS_1 : public Shader_GL
{
public:
    BEGIN_SHADER_PARAM_STRUCT()
    END_SHADER_PARAM_STRUCT()

    DECLARE_SHADER(LightFS_1)
};
IMPLEMENT_SHADER(LightFS_1, Root_Path + "source/shaders/1_light.fs", Shader_Type::FRAGMENT_SHADER)

class PostVS_2 : public Shader_GL
{
public:
    BEGIN_SHADER_PARAM_STRUCT()
    END_SHADER_PARAM_STRUCT()

    DECLARE_SHADER(PostVS_2)
};
IMPLEMENT_SHADER(PostVS_2, Root_Path + "source/shaders/2_post.vs", Shader_Type::VERTEX_SHADER)

class LuminanceFS_2 : public Shader_GL
{
public:
    BEGIN_SHADER_PARAM_STRUCT()
    SHADER_PARAM(SDTexture2D, texture1)
    END_SHADER_PARAM_STRUCT()

    DECLARE_SHADER(LuminanceFS_2)
};
IMPLEMENT_SHADER(LuminanceFS_2, Root_Path + "source/shaders/2_luminance.fs", Shader_Type::FRAGMENT_SHADER)

class BoomFS_2 : public Shader_GL
{
public:
    BEGIN_SHADER_PARAM_STRUCT()
    SHADER_PARAM(SDTexture2D, texture1)
    SHADER_PARAM(SDFloat,lod)
    END_SHADER_PARAM_STRUCT()

    DECLARE_SHADER(BoomFS_2)
};
IMPLEMENT_SHADER(BoomFS_2, Root_Path + "source/shaders/2_boom.fs", Shader_Type::FRAGMENT_SHADER)
