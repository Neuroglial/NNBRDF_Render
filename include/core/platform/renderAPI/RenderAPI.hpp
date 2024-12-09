#pragma once

#include <iostream>
#include <unordered_map>
#include <functional>

#include "core/platform/renderAPI/Texture.hpp"
#include "core/platform/renderAPI/Shader.hpp"
#include "core/platform/renderAPI/ArrayBuffer.hpp"
#include "core/platform/renderAPI/UniformBuffer.hpp"
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
        static type *crt()                                                     \
        {                                                                      \
            auto api = *get_graphic_api();                                     \
            if (api == GraphicsAPI::OpenGL)                                    \
            {                                                                  \
                auto i = (*get_create_map()).find(std::string(#type) + "_GL"); \
                if (i == (*get_create_map()).end())                            \
                {                                                              \
                    throw std::runtime_error("Create Function Not Founded");   \
                }                                                              \
                return (type *)i->second();                                    \
            }                                                                  \
            else                                                               \
            {                                                                  \
                throw std::runtime_error("Unknown API");                       \
                return nullptr;                                                \
            }                                                                  \
        }                                                                      \
    };

//#define API_REG_LOG(x) std::cout << "API Log: " << x << " Registered" << std::endl;
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
    GraphicsAPI *get_graphic_api();

    void init(GraphicsAPI api);

    func_map *get_create_map();

    void register_create(const std::string name, std::function<void *()> fun);

    template <typename T>
    struct creator
    {
        static T *crt()
        {
            return nullptr;
        }
    };

    CREATOR_HELPER(Shader)
    CREATOR_HELPER(Pipeline)
}
