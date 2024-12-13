#include "core/platform/renderAPI/RenderAPI.hpp"

namespace RenderAPI
{
    Ref<Base> base_api = nullptr;

    GraphicsAPI *get_graphic_api()
    {
        static GraphicsAPI api;
        return &api;
    }

    void init(GraphicsAPI api)
    {
        *get_graphic_api() = api;
        base_api = creator<Base>::crt();
        base_api->init();
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

    void viewport(int bx, int by, int width, int height)
    {
        base_api->viewport(bx, by, width, height);
    }
    void viewport(int width, int height)
    {
        base_api->viewport(0, 0, width, height);
    }

    void clear()
    {
        base_api->clear();
    }
    void clear(const glm::vec4 &color)
    {
        base_api->clear(color);
    }

    void depth_test(bool enable)
    {
        base_api->depth_test(enable);
    }

    void face_culling(bool enable, bool back_culling)
    {
        base_api->face_culling(enable, back_culling);
    }
}