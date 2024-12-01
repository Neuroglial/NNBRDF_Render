#pragma once

#include "core/platform/renderAPI/OpenGL/Shader_GL.hpp"
#include "scene/ShaderManager.hpp"

#ifdef __SHADER_MANAGER__
    #define IMPLEMENT_SHADER(ClassName,Path,ShaderType) ClassName ClassName::instance(Path,ShaderType);
#else
    #define IMPLEMENT_SHADER(ClassName,Path,ShaderType)
#endif



namespace Shader{
    class VertexShader:public Shader_GL{
        public:

        BEGIN_SHADER_PARAM_STRUCT()
            SHADER_PARAM(SDVec3,color1)
        END_SHADER_PARAM_STRUCT()

        DECLARE_SHADER(VertexShader)
    };
    IMPLEMENT_SHADER(VertexShader,"./3.3.shader.vs",Shader_Type::VERTEX_SHADER)

    class FragmentShader:public Shader_GL{
        public:

        BEGIN_SHADER_PARAM_STRUCT()
            SHADER_PARAM(SDVec3,color2)
        END_SHADER_PARAM_STRUCT()

        DECLARE_SHADER(FragmentShader)
    };
    IMPLEMENT_SHADER(FragmentShader,"./3.3.shader.fs",Shader_Type::FRAGMENT_SHADER)
}