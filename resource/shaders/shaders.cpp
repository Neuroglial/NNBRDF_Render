// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#include "scene/ShaderManager.hpp"
#include "core/platform/renderAPI/RenderAPI.hpp"
#include "core/platform/renderAPI/ShaderParam.hpp"

// shader文件命名规则：接口类型(fs同vs)_名称_pn(n为第几pass).vs(fs)
// shader类命名规则：文件名将"."改为"_"

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
    REGISTER_SHADER(a_default_vs, Root_Path + "resource/shaders/a_default.vs", Shader_Type::VERTEX_SHADER)
*/

class a_default_vs
{
public:
    BEGIN_SHADER_PARAM_STRUCT()
    SHADER_PARAM(SDMat4, model)
    END_SHADER_PARAM_STRUCT()

    DECLARE_SHADER(a_default_vs)
};
REGISTER_SHADER(a_default_vs, Root_Path + "resource/shaders/a_default.vs", Shader_Type::VERTEX_SHADER)

class a_default_fs
{
public:
    BEGIN_SHADER_PARAM_STRUCT()
    SHADER_PARAM(SDTexture2D, texture1)
    SHADER_PARAM(SDTexture2D, texture2)
    END_SHADER_PARAM_STRUCT()

    DECLARE_SHADER(a_default_fs)
};
REGISTER_SHADER(a_default_fs, Root_Path + "resource/shaders/a_default.fs", Shader_Type::FRAGMENT_SHADER)

class a_light_fs
{
public:
    BEGIN_SHADER_PARAM_STRUCT()
    END_SHADER_PARAM_STRUCT()

    DECLARE_SHADER(a_light_fs)
};
REGISTER_SHADER(a_light_fs, Root_Path + "resource/shaders/a_light.fs", Shader_Type::FRAGMENT_SHADER)

class a_skybox_hdr_fs
{
public:
    BEGIN_SHADER_PARAM_STRUCT()
    SHADER_PARAM(SDTexture2D, iChannel0)
    END_SHADER_PARAM_STRUCT()

    DECLARE_SHADER(a_skybox_hdr_fs)
};
REGISTER_SHADER(a_skybox_hdr_fs, Root_Path + "resource/shaders/a_skybox_hdr.fs", Shader_Type::FRAGMENT_SHADER)

class a_skybox_cubemap_fs
{
public:
    BEGIN_SHADER_PARAM_STRUCT()
    SHADER_PARAM(SDTextureCube, iChannel0)
    END_SHADER_PARAM_STRUCT()

    DECLARE_SHADER(a_skybox_cubemap_fs)
};
REGISTER_SHADER(a_skybox_cubemap_fs, Root_Path + "resource/shaders/a_skybox_cubemap.fs", Shader_Type::FRAGMENT_SHADER)

class a_Blinn_Phong_BRDF_fs
{
public:
    BEGIN_SHADER_PARAM_STRUCT()
    SHADER_PARAM(SDTexture2D, mt_diffuse)
    SHADER_PARAM(SDTexture2D, mt_specular)
    SHADER_PARAM(SDFloat, mt_shininess)
    SHADER_PARAM_ARRAY(SDTextureCube_Array, depthMap, 1)
    SHADER_PARAM(SDFloat, far_plane)
    SHADER_PARAM(SDVec3, lightPos)
    END_SHADER_PARAM_STRUCT()

    DECLARE_SHADER(a_Blinn_Phong_BRDF_fs)
};
REGISTER_SHADER(a_Blinn_Phong_BRDF_fs, Root_Path + "resource/shaders/a_Blinn_Phong_BRDF.fs", Shader_Type::FRAGMENT_SHADER)

class a_void_fs
{
public:
    BEGIN_SHADER_PARAM_STRUCT()
    END_SHADER_PARAM_STRUCT()

    DECLARE_SHADER(a_void_fs)
};
REGISTER_SHADER(a_void_fs, Root_Path + "resource/shaders/a_void.fs", Shader_Type::FRAGMENT_SHADER)

class b_post_vs
{
public:
    BEGIN_SHADER_PARAM_STRUCT()
    END_SHADER_PARAM_STRUCT()

    DECLARE_SHADER(b_post_vs)
};
REGISTER_SHADER(b_post_vs, Root_Path + "resource/shaders/b_post.vs", Shader_Type::VERTEX_SHADER)

class b_luminance_fs
{
public:
    BEGIN_SHADER_PARAM_STRUCT()
    SHADER_PARAM(SDTexture2D, texture1)
    END_SHADER_PARAM_STRUCT()

    DECLARE_SHADER(b_luminance_fs)
};
REGISTER_SHADER(b_luminance_fs, Root_Path + "resource/shaders/b_luminance.fs", Shader_Type::FRAGMENT_SHADER)

class b_blackhole_p1_fs
{
public:
    BEGIN_SHADER_PARAM_STRUCT()
    SHADER_PARAM(SDTexture2D, iChannel0)
    SHADER_PARAM(SDTexture2D, iChannel1)
    SHADER_PARAM(SDTexture2D, iChannel2)
    END_SHADER_PARAM_STRUCT()

    DECLARE_SHADER(b_blackhole_p1_fs)
};
REGISTER_SHADER(b_blackhole_p1_fs, Root_Path + "resource/shaders/b_blackhole_p1.fs", Shader_Type::FRAGMENT_SHADER)

class b_blackhole_p2_fs
{
public:
    BEGIN_SHADER_PARAM_STRUCT()
    SHADER_PARAM(SDTexture2D, iChannel0)
    END_SHADER_PARAM_STRUCT()

    DECLARE_SHADER(b_blackhole_p2_fs)
};
REGISTER_SHADER(b_blackhole_p2_fs, Root_Path + "resource/shaders/b_blackhole_p2.fs", Shader_Type::FRAGMENT_SHADER)

class b_blackhole_p3_fs
{
public:
    BEGIN_SHADER_PARAM_STRUCT()
    SHADER_PARAM(SDTexture2D, iChannel0)
    END_SHADER_PARAM_STRUCT()

    DECLARE_SHADER(b_blackhole_p3_fs)
};
REGISTER_SHADER(b_blackhole_p3_fs, Root_Path + "resource/shaders/b_blackhole_p3.fs", Shader_Type::FRAGMENT_SHADER)

class b_blackhole_p4_fs
{
public:
    BEGIN_SHADER_PARAM_STRUCT()
    SHADER_PARAM(SDTexture2D, iChannel0)
    END_SHADER_PARAM_STRUCT()

    DECLARE_SHADER(b_blackhole_p4_fs)
};
REGISTER_SHADER(b_blackhole_p4_fs, Root_Path + "resource/shaders/b_blackhole_p4.fs", Shader_Type::FRAGMENT_SHADER)

class b_blackhole_p5_fs
{
public:
    BEGIN_SHADER_PARAM_STRUCT()
    SHADER_PARAM(SDTexture2D, iChannel0)
    SHADER_PARAM(SDTexture2D, iChannel1)
    SHADER_PARAM(SDTexture2D, iChannel2)
    SHADER_PARAM(SDTexture2D, iChannel3)
    END_SHADER_PARAM_STRUCT()

    DECLARE_SHADER(b_blackhole_p5_fs)
};
REGISTER_SHADER(b_blackhole_p5_fs, Root_Path + "resource/shaders/b_blackhole_p5.fs", Shader_Type::FRAGMENT_SHADER)

class b_through_fs
{
public:
    BEGIN_SHADER_PARAM_STRUCT()
    SHADER_PARAM(SDTexture2D, iChannel0)
    END_SHADER_PARAM_STRUCT()

    DECLARE_SHADER(b_through_fs)
};
REGISTER_SHADER(b_through_fs, Root_Path + "resource/shaders/b_through.fs", Shader_Type::FRAGMENT_SHADER)

class b_depth_test_fs
{
public:
    BEGIN_SHADER_PARAM_STRUCT()
    SHADER_PARAM(SDTexture2D, iChannel0)
    SHADER_PARAM(SDTexture2D, iChannel1)
    SHADER_PARAM(SDFloat, depth)
    END_SHADER_PARAM_STRUCT()

    DECLARE_SHADER(b_depth_test_fs)
};
REGISTER_SHADER(b_depth_test_fs, Root_Path + "resource/shaders/b_depth_test.fs", Shader_Type::FRAGMENT_SHADER)

class b_copy_fs
{
public:
    BEGIN_SHADER_PARAM_STRUCT()
    SHADER_PARAM(SDTexture2D, iChannel0)
    END_SHADER_PARAM_STRUCT()

    DECLARE_SHADER(b_copy_fs)
};
REGISTER_SHADER(b_copy_fs, Root_Path + "resource/shaders/b_copy.fs", Shader_Type::FRAGMENT_SHADER)

class b_boom_fs
{
public:
    BEGIN_SHADER_PARAM_STRUCT()
    SHADER_PARAM(SDTexture2D, texture1)
    SHADER_PARAM(SDFloat, lod)
    END_SHADER_PARAM_STRUCT()

    DECLARE_SHADER(b_boom_fs)
};
REGISTER_SHADER(b_boom_fs, Root_Path + "resource/shaders/b_boom.fs", Shader_Type::FRAGMENT_SHADER)

class c_point_shadow_vs
{
public:
    BEGIN_SHADER_PARAM_STRUCT()
    SHADER_PARAM(SDMat4, model)
    END_SHADER_PARAM_STRUCT()

    DECLARE_SHADER(c_point_shadow_vs)
};
REGISTER_SHADER(c_point_shadow_vs, Root_Path + "resource/shaders/c_point_shadow.vs", Shader_Type::VERTEX_SHADER)

class c_point_shadow_gs
{
public:
    BEGIN_SHADER_PARAM_STRUCT()
    SHADER_PARAM(SDMat4, shadowMat_0)
    SHADER_PARAM(SDMat4, shadowMat_1)
    SHADER_PARAM(SDMat4, shadowMat_2)
    SHADER_PARAM(SDMat4, shadowMat_3)
    SHADER_PARAM(SDMat4, shadowMat_4)
    SHADER_PARAM(SDMat4, shadowMat_5)
    END_SHADER_PARAM_STRUCT()

    DECLARE_SHADER(c_point_shadow_gs)
};
REGISTER_SHADER(c_point_shadow_gs, Root_Path + "resource/shaders/c_point_shadow.gs", Shader_Type::GEOMETRY_SHADER)

class c_point_shadow_fs
{
public:
    BEGIN_SHADER_PARAM_STRUCT()
    SHADER_PARAM(SDVec3, lightPos)
    SHADER_PARAM(SDFloat, far_plane)
    END_SHADER_PARAM_STRUCT()

    DECLARE_SHADER(c_point_shadow_fs)
};
REGISTER_SHADER(c_point_shadow_fs, Root_Path + "resource/shaders/c_point_shadow.fs", Shader_Type::FRAGMENT_SHADER)