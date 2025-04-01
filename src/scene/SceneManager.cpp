#include "scene/SceneManger.hpp"
#include "scene/Serialize.hpp"
#include "scene/CameraUnifrom.hpp"

void SceneManager::loadScene(const std::string &path)
{
    // to_file(scene_mgr, "resource/scene/test1.scene");
    deleteScene();
    from_json_ptr(from_file_json(path), this);
    Start();
}

void SceneManager::saveScene(const std::string &path)
{
    // to_file(scene_mgr, "resource/scene/test1.scene");
    to_file(*this, path);
}

void SceneManager::deleteScene()
{
    m_registry.view<ScriptComponent>().each(
        [](ScriptComponent &sc)
        {
            if (sc.m_script)
                sc.m_script->OnDestroy();
        });

    m_registry.clear();
    m_objects.clear();
    m_delete.clear();
    m_distributor.clear();
    m_root.clear();
    m_ActiveCamera = nullptr;

}

void SceneManager::update_camera()
{
    bool haveActive = false;
    m_registry.view<TransformComponent, CameraComponet>().each(
        [&](TransformComponent &trans, CameraComponet &camera)
        {
            // The default camera is facing the negative Z-axis direction. To match intuition,
            //  rotate 180 degrees to make it face the positive Z-axis direction
            const static glm::mat4 rot180_Y = utils::get_rotation(glm::vec3(0, 180, 0));
            camera.m_view = rot180_Y * glm::inverse(trans.m_model);

            if (camera.m_Active && !haveActive)
            {
                CameraUniform::bind(&camera);
                haveActive = true;
                m_ActiveCamera = camera.gameObject;
            }
            else if (haveActive)
            {
                camera.m_Active = false;
            }
        });

    if (!haveActive)
        m_ActiveCamera = false;
}