#pragma once
#include <entt/entt.hpp>
#include "utils/utils.hpp"
#include "core/platform/renderAPI/RenderAPI.hpp"
#include "core/render/Material.hpp"
#include "core/platform/system/EventManager.hpp"

class ScriptManager;
class GameObject;

struct ComponentBase
{
    GameObject *gameObject;
};

class ScriptBase
{
public:
    virtual void Start() {};
    virtual void Update(float delta) {};
    virtual void OnDestroy() {};
    virtual void CallBack(Event::Event &event) {};

    GameObject *gameObject;
    friend class ScriptManager;
};

struct ScriptComponent : public ComponentBase
{
    Ref<ScriptBase> script;

    void Start()
    {
        if (script)
            script->Start();
    }

    void Update(float delta)
    {
        if (script)
            script->Update(delta);
    }

    void OnDestroy()
    {
        if (script)
        {
            script->OnDestroy();
            script = nullptr;
        }
    }
};

struct TransformComponent : public ComponentBase
{
    glm::vec3 m_pos = glm::vec3(0);
    glm::vec3 m_scl = glm::vec3(1);
    glm::quat m_rot = glm::quat(glm::mat3(1));

    glm::mat4 m_model = glm::mat4(0);

    bool m_static = false;

    GameObject *m_father = nullptr;
    std::vector<GameObject *> m_children;

    void attach(TransformComponent *father);
    void detach();
    glm::vec3 get_world_pos();
    glm::vec3 get_right();
    glm::vec3 get_rotEuler() { return utils::to_euler(m_rot); }
    void set_rotEuler(const glm::vec3 &degrees) { m_rot = utils::to_quat(degrees); }

    void rotate_local(const glm::vec3 &degrees)
    {
        m_rot = m_rot * utils::to_quat(degrees);
    }

    void rotate_world(const glm::vec3 &degrees)
    {
        m_rot = utils::to_quat(degrees) * m_rot;
    }

    glm::vec3 get_forword();
    void get_allChildren(std::vector<GameObject *> &children);
    bool have_child(GameObject *object);
};

struct MeshComponent : public ComponentBase
{
    Ref<Mesh> m_mesh;
};

struct RendererComponent : public ComponentBase
{
    std::vector<Ref<Material>> m_materials;

    void Render();
};

struct PointLightComponent : public ComponentBase
{
};

struct CameraComponet : public ComponentBase
{
    enum ProjectMode
    {
        Ortho,
        Persp,
    };

    ProjectMode m_projMode = Persp;
    float m_fov = 45.0f;
    float m_near = 0.01f;
    float m_far = 100.0f;
    glm::mat4 m_view = glm::mat4(1);

    glm::mat4 get_proj();
    glm::vec3 get_pos();
};