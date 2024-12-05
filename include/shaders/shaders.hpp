#pragma once

#include "core/platform/renderAPI/Shader.hpp"
#include "core/platform/renderAPI/OpenGL/Shader_GL.hpp"
#include "scene/ShaderManager.hpp"

#ifdef __SHADER_MANAGER__
    #define IMPLEMENT_SHADER(ClassName,Path,ShaderType) ClassName ClassName::instance(Path,ShaderType);
#else
    #define IMPLEMENT_SHADER(ClassName,Path,ShaderType)
#endif

using namespace Shader;

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




class CubeVS:public Shader_GL{
    public:

    BEGIN_SHADER_PARAM_STRUCT()
        SHADER_PARAM(SDMat4,model)
        SHADER_PARAM(SDMat4,view)
        SHADER_PARAM(SDMat4,projection)
    END_SHADER_PARAM_STRUCT()

    DECLARE_SHADER(CubeVS)
};
IMPLEMENT_SHADER(CubeVS,"../source/shaders/cube.vs",Shader_Type::VERTEX_SHADER)

class CubeFS:public Shader_GL{
    public:

    BEGIN_SHADER_PARAM_STRUCT()
        SHADER_PARAM(SDTexture2D,texture1)
        SHADER_PARAM(SDTexture2D,texture2)
        SHADER_PARAM(SDVec3,lightPos)
        SHADER_PARAM(SDVec3,viewPos)
        SHADER_PARAM(SDVec3,lightColor)
    END_SHADER_PARAM_STRUCT()

    DECLARE_SHADER(CubeFS)
};
IMPLEMENT_SHADER(CubeFS,"../source/shaders/cube.fs",Shader_Type::FRAGMENT_SHADER)

class LightFS:public Shader_GL{
    public:

    BEGIN_SHADER_PARAM_STRUCT()
        SHADER_PARAM(SDVec3,lightColor)
    END_SHADER_PARAM_STRUCT()

    DECLARE_SHADER(LightFS)
};
IMPLEMENT_SHADER(LightFS,"../source/shaders/light.fs",Shader_Type::FRAGMENT_SHADER)
