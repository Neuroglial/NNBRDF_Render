// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#pragma once
#include "core/platform/renderAPI/Base.hpp"

#include <iostream>
#include <unordered_map>
#include <functional>

#include "core/platform/renderAPI/Texture.hpp"
#include "core/platform/renderAPI/Shader.hpp"
#include "core/platform/renderAPI/ArrayBuffer.hpp"
#include "core/platform/renderAPI/Mesh.hpp"
#include "core/platform/renderAPI/OpenGL/ArrayBuffer_GL.hpp"
#include "core/platform/renderAPI/UniformBuffer.hpp"
#include "core/platform/renderAPI/FrameBuffer.hpp"
#include "utils/utils.hpp"

enum class GraphicsAPI
{
    None,
    OpenGL,
};

using func_map = std::unordered_map<std::string, std::function<void *()>>;

#define CREATOR_HELPER(type)                                                   \
    template <>                                                                \
    struct creator<type>                                                       \
    {                                                                          \
        static Ref<type> crt()                                                 \
        {                                                                      \
            auto api = *get_graphic_api();                                     \
            if (api == GraphicsAPI::OpenGL)                                    \
            {                                                                  \
                auto i = (*get_create_map()).find(std::string(#type) + "_GL"); \
                if (i == (*get_create_map()).end())                            \
                {                                                              \
                    throw std::runtime_error("Create Function Not Founded");   \
                }                                                              \
                return Ref<type>((type *)i->second());                         \
            }                                                                  \
            else                                                               \
            {                                                                  \
                throw std::runtime_error("Unknown API");                       \
                return nullptr;                                                \
            }                                                                  \
        }                                                                      \
    };

// #define API_REG_LOG(x) std::cout << "API Log: " << x << " Registered" << std::endl;
#define API_REG_LOG(x)

#define REGISTER_API(type)                                                                    \
    struct REGISTER_##type                                                                    \
    {                                                                                         \
        REGISTER_##type()                                                                     \
        {                                                                                     \
            API_REG_LOG(#type);                                                               \
            RenderAPI::register_create(#type, []() -> void * { return (void *)new type(); }); \
        }                                                                                     \
    };                                                                                        \
    REGISTER_##type INS_##type;

namespace RenderAPI
{
    extern Ref<Base> base_api;

    GraphicsAPI *get_graphic_api();

    void init(GraphicsAPI api);

    func_map *get_create_map();

    void register_create(const std::string name, std::function<void *()> fun);

    template <typename T>
    struct creator
    {
        static Ref<T> crt()
        {
            return nullptr;
        }
    };

    CREATOR_HELPER(Shader)
    CREATOR_HELPER(Pipeline)
    CREATOR_HELPER(Mesh)
    CREATOR_HELPER(Texture2D)
    CREATOR_HELPER(TextureCube)
    CREATOR_HELPER(UniformBuffer)
    CREATOR_HELPER(FrameBuffer)
    CREATOR_HELPER(Base)

    //TODO Template classes currently do not have a good reflection method
    template <typename T>
    struct creator<ArrayBuffer<T>>
    {
        static Ref<ArrayBuffer<T>> crt()
        {
            auto api = *get_graphic_api();
            if (api == GraphicsAPI::OpenGL)
            {
                auto ptr = new ArrayBuffer_GL<T>();
                return Ref<ArrayBuffer<T>>(ptr);
            }
            else
            {
                throw std::runtime_error("Unknown API");
                return nullptr;
            }
        }
    };

    void viewport(int bx, int by, int width, int height);
    void viewport(int width, int height);

    glm::vec2 get_frameBufferSize();
    void get_frameBufferSize(int &width,int& height);

    void clear();
    void clear(const glm::vec4 &color);
    void depth_test(bool enable);
    void face_culling(bool enable, bool back_culling = true);
}
