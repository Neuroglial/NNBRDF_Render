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
    class VertexShader:public Shader_GL{
        public:

        BEGIN_SHADER_PARAM_STRUCT()
            SHADER_PARAM(SDVec3,color1)
        END_SHADER_PARAM_STRUCT()

        DECLARE_SHADER(VertexShader)
    };
    IMPLEMENT_SHADER(VertexShader,"../3.3.shader.vs",Shader_Type::VERTEX_SHADER)
*/




class VertexShader:public Shader_GL{
    public:

    BEGIN_SHADER_PARAM_STRUCT()
    END_SHADER_PARAM_STRUCT()

    DECLARE_SHADER(VertexShader)
};
IMPLEMENT_SHADER(VertexShader,"../vertex_shader.glsl",Shader_Type::VERTEX_SHADER)

class FragmentShader:public Shader_GL{
    public:

    BEGIN_SHADER_PARAM_STRUCT()
        SHADER_PARAM(SDInt,texture1)
        SHADER_PARAM(SDInt,texture2)
        SHADER_PARAM(SDVec3,color1)
    END_SHADER_PARAM_STRUCT()

    DECLARE_SHADER(FragmentShader)
};
IMPLEMENT_SHADER(FragmentShader,"../fragment_shader.glsl",Shader_Type::FRAGMENT_SHADER)
