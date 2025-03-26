#include "scene/SceneManger.hpp"
#include "scene/Serialize.hpp"
#include "scene/CameraUnifrom.hpp"

void SceneManager::loadScene(const std::string &path)
{
    // to_file(scene_mgr, "resource/scene/test1.scene");
    from_json_ptr(from_file_json("resource/scene/test1.scene"), this);
}

void SceneManager::bind_camera()
{
    m_registry.view<CameraComponet>().each(
        [](CameraComponet &camera)
        {
            if (camera.m_Active)
            {
                CameraUniform::bind(&camera);
            }
        });
}