// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#include "scene/ShaderManager.hpp"
#include "core/platform/renderAPI/RenderAPI.hpp"
#include "core/platform/renderAPI/Param.hpp"

// shader File naming rules: interface type (fs is the same as vs)_name_pn (n is the pass number).vs(fs)
// shader Class naming rules: Change the file name from "." to "_"

/*
    Declare the shader here in the following format:
    class a_default_vs  //Name with file name_vs(fs) for easy searching
    {
    public:
        BEGIN_SHADER_PARAM_STRUCT()  // Define parameters with SD as the prefix to support parameter types
            PARAM(SDMat4, model)
        END_SHADER_PARAM_STRUCT()

        DECLARE_SHADER(a_default_vs)
    };
    REGISTER_SHADER(a_default_vs, Root_Path + "resource/shaders/a_default.vs", ParamType::VERTEX_SHADER)
*/

// class a_default_vs
// {
// public:
//     BEGIN_SHADER_PARAM_STRUCT()
//     PARAM(SDMat4, model)
//     END_SHADER_PARAM_STRUCT()

//     DECLARE_SHADER(a_default_vs)
// };
// REGISTER_SHADER(a_default_vs, Root_Path + "resource/shaders/a_default.vs", ParamType::VERTEX_SHADER)

// class a_default_fs
// {
// public:
//     BEGIN_SHADER_PARAM_STRUCT()
//     PARAM(SDTexture2D, texture1)
//     PARAM(SDTexture2D, texture2)
//     END_SHADER_PARAM_STRUCT()

//     DECLARE_SHADER(a_default_fs)
// };
// REGISTER_SHADER(a_default_fs, Root_Path + "resource/shaders/a_default.fs", ParamType::FRAGMENT_SHADER)

// class a_light_fs
// {
// public:
//     BEGIN_SHADER_PARAM_STRUCT()
//     END_SHADER_PARAM_STRUCT()

//     DECLARE_SHADER(a_light_fs)
// };
// REGISTER_SHADER(a_light_fs, Root_Path + "resource/shaders/a_light.fs", ParamType::FRAGMENT_SHADER)

// class a_skybox_hdr_fs
// {
// public:
//     BEGIN_SHADER_PARAM_STRUCT()
//     PARAM(SDTexture2D, iChannel0)
//     END_SHADER_PARAM_STRUCT()

//     DECLARE_SHADER(a_skybox_hdr_fs)
// };
// REGISTER_SHADER(a_skybox_hdr_fs, Root_Path + "resource/shaders/a_skybox_hdr.fs", ParamType::FRAGMENT_SHADER)

// class a_skybox_cubemap_fs
// {
// public:
//     BEGIN_SHADER_PARAM_STRUCT()
//     PARAM(SDTextureCube, iChannel0)
//     END_SHADER_PARAM_STRUCT()

//     DECLARE_SHADER(a_skybox_cubemap_fs)
// };
// REGISTER_SHADER(a_skybox_cubemap_fs, Root_Path + "resource/shaders/a_skybox_cubemap.fs", ParamType::FRAGMENT_SHADER)

// class a_Blinn_Phong_BRDF_fs
// {
// public:
//     BEGIN_SHADER_PARAM_STRUCT()
//     PARAM(SDTexture2D, mt_diffuse)
//     PARAM(SDTexture2D, mt_specular)
//     PARAM(SDFloat, mt_shininess)
//     SHADER_PARAM_ARRAY(SDTextureCube, depthMap, 1)
//     PARAM(SDFloat, far_plane)
//     PARAM(SDVec3, lightPos)
//     END_SHADER_PARAM_STRUCT()

//     DECLARE_SHADER(a_Blinn_Phong_BRDF_fs)
// };
// REGISTER_SHADER(a_Blinn_Phong_BRDF_fs, Root_Path + "resource/shaders/a_Blinn_Phong_BRDF.fs", ParamType::FRAGMENT_SHADER)

// class a_void_fs
// {
// public:
//     BEGIN_SHADER_PARAM_STRUCT()
//     END_SHADER_PARAM_STRUCT()

//     DECLARE_SHADER(a_void_fs)
// };
// REGISTER_SHADER(a_void_fs, Root_Path + "resource/shaders/a_void.fs", ParamType::FRAGMENT_SHADER)

// class b_post_vs
// {
// public:
//     BEGIN_SHADER_PARAM_STRUCT()
//     END_SHADER_PARAM_STRUCT()

//     DECLARE_SHADER(b_post_vs)
// };
// REGISTER_SHADER(b_post_vs, Root_Path + "resource/shaders/b_post.vs", ParamType::VERTEX_SHADER)

// class b_luminance_fs
// {
// public:
//     BEGIN_SHADER_PARAM_STRUCT()
//     PARAM(SDTexture2D, texture1)
//     END_SHADER_PARAM_STRUCT()

//     DECLARE_SHADER(b_luminance_fs)
// };
// REGISTER_SHADER(b_luminance_fs, Root_Path + "resource/shaders/b_luminance.fs", ParamType::FRAGMENT_SHADER)

// class b_blackhole_p1_fs
// {
// public:
//     BEGIN_SHADER_PARAM_STRUCT()
//     PARAM(SDTexture2D, iChannel0)
//     PARAM(SDTexture2D, iChannel1)
//     PARAM(SDTexture2D, iChannel2)
//     END_SHADER_PARAM_STRUCT()

//     DECLARE_SHADER(b_blackhole_p1_fs)
// };
// REGISTER_SHADER(b_blackhole_p1_fs, Root_Path + "resource/shaders/b_blackhole_p1.fs", ParamType::FRAGMENT_SHADER)

// class b_blackhole_p2_fs
// {
// public:
//     BEGIN_SHADER_PARAM_STRUCT()
//     PARAM(SDTexture2D, iChannel0)
//     END_SHADER_PARAM_STRUCT()

//     DECLARE_SHADER(b_blackhole_p2_fs)
// };
// REGISTER_SHADER(b_blackhole_p2_fs, Root_Path + "resource/shaders/b_blackhole_p2.fs", ParamType::FRAGMENT_SHADER)

// class b_blackhole_p3_fs
// {
// public:
//     BEGIN_SHADER_PARAM_STRUCT()
//     PARAM(SDTexture2D, iChannel0)
//     END_SHADER_PARAM_STRUCT()

//     DECLARE_SHADER(b_blackhole_p3_fs)
// };
// REGISTER_SHADER(b_blackhole_p3_fs, Root_Path + "resource/shaders/b_blackhole_p3.fs", ParamType::FRAGMENT_SHADER)

// class b_blackhole_p4_fs
// {
// public:
//     BEGIN_SHADER_PARAM_STRUCT()
//     PARAM(SDTexture2D, iChannel0)
//     END_SHADER_PARAM_STRUCT()

//     DECLARE_SHADER(b_blackhole_p4_fs)
// };
// REGISTER_SHADER(b_blackhole_p4_fs, Root_Path + "resource/shaders/b_blackhole_p4.fs", ParamType::FRAGMENT_SHADER)

// class b_blackhole_p5_fs
// {
// public:
//     BEGIN_SHADER_PARAM_STRUCT()
//     PARAM(SDTexture2D, iChannel0)
//     PARAM(SDTexture2D, iChannel1)
//     PARAM(SDTexture2D, iChannel2)
//     PARAM(SDTexture2D, iChannel3)
//     END_SHADER_PARAM_STRUCT()

//     DECLARE_SHADER(b_blackhole_p5_fs)
// };
// REGISTER_SHADER(b_blackhole_p5_fs, Root_Path + "resource/shaders/b_blackhole_p5.fs", ParamType::FRAGMENT_SHADER)

// class b_through_fs
// {
// public:
//     BEGIN_SHADER_PARAM_STRUCT()
//     PARAM(SDTexture2D, iChannel0)
//     END_SHADER_PARAM_STRUCT()

//     DECLARE_SHADER(b_through_fs)
// };
// REGISTER_SHADER(b_through_fs, Root_Path + "resource/shaders/b_through.fs", ParamType::FRAGMENT_SHADER)

// class b_depth_test_fs
// {
// public:
//     BEGIN_SHADER_PARAM_STRUCT()
//     PARAM(SDTexture2D, iChannel0)
//     PARAM(SDTexture2D, iChannel1)
//     PARAM(SDFloat, depth)
//     END_SHADER_PARAM_STRUCT()

//     DECLARE_SHADER(b_depth_test_fs)
// };
// REGISTER_SHADER(b_depth_test_fs, Root_Path + "resource/shaders/b_depth_test.fs", ParamType::FRAGMENT_SHADER)

// class b_copy_fs
// {
// public:
//     BEGIN_SHADER_PARAM_STRUCT()
//     PARAM(SDTexture2D, iChannel0)
//     END_SHADER_PARAM_STRUCT()

//     DECLARE_SHADER(b_copy_fs)
// };
// REGISTER_SHADER(b_copy_fs, Root_Path + "resource/shaders/b_copy.fs", ParamType::FRAGMENT_SHADER)

// class b_boom_fs
// {
// public:
//     BEGIN_SHADER_PARAM_STRUCT()
//     PARAM(SDTexture2D, texture1)
//     PARAM(SDFloat, lod)
//     END_SHADER_PARAM_STRUCT()

//     DECLARE_SHADER(b_boom_fs)
// };
// REGISTER_SHADER(b_boom_fs, Root_Path + "resource/shaders/b_boom.fs", ParamType::FRAGMENT_SHADER)

// class c_point_shadow_vs
// {
// public:
//     BEGIN_SHADER_PARAM_STRUCT()
//     PARAM(SDMat4, model)
//     END_SHADER_PARAM_STRUCT()

//     DECLARE_SHADER(c_point_shadow_vs)
// };
// REGISTER_SHADER(c_point_shadow_vs, Root_Path + "resource/shaders/c_point_shadow.vs", ParamType::VERTEX_SHADER)

// class c_point_shadow_gs
// {
// public:
//     BEGIN_SHADER_PARAM_STRUCT()
//     PARAM(SDMat4, shadowMat_0)
//     PARAM(SDMat4, shadowMat_1)
//     PARAM(SDMat4, shadowMat_2)
//     PARAM(SDMat4, shadowMat_3)
//     PARAM(SDMat4, shadowMat_4)
//     PARAM(SDMat4, shadowMat_5)
//     END_SHADER_PARAM_STRUCT()

//     DECLARE_SHADER(c_point_shadow_gs)
// };
// REGISTER_SHADER(c_point_shadow_gs, Root_Path + "resource/shaders/c_point_shadow.gs", ParamType::GEOMETRY_SHADER)

// class c_point_shadow_fs
// {
// public:
//     BEGIN_SHADER_PARAM_STRUCT()
//     PARAM(SDVec3, lightPos)
//     PARAM(SDFloat, far_plane)
//     END_SHADER_PARAM_STRUCT()

//     DECLARE_SHADER(c_point_shadow_fs)
// };
// REGISTER_SHADER(c_point_shadow_fs, Root_Path + "resource/shaders/c_point_shadow.fs", ParamType::FRAGMENT_SHADER)