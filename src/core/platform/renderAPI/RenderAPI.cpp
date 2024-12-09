#include "core/platform/renderAPI/RenderAPI.hpp"
#include <glad/glad.h>
#include <GLFW/glfw3.h>

namespace RenderAPI
{
    GraphicsAPI *get_graphic_api()
    {
        static GraphicsAPI api;
        return &api;
    }

    void init(GraphicsAPI api)
    {
        *get_graphic_api() = api;
        
        if (api == GraphicsAPI::OpenGL)
        {
            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
            {
                throw std::runtime_error("Failed to initialize GLAD");
            }
        }
    }

    func_map *get_create_map()
    {
        static func_map map;
        return &map;
    }

    void register_create(const std::string name, std::function<void *()> fun)
    {
        auto map = get_create_map();
        map->insert(std::pair<std::string, std::function<void *()>>(name, fun));
    }
}