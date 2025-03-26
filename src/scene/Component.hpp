#pragma once
#include <entt/entt.hpp>
#include "utils/utils.hpp"
#include "core/platform/renderAPI/RenderAPI.hpp"
#include "core/render/Material.hpp"
#include "core/platform/system/EventManager.hpp"
#include "scene/LightUniform.hpp"

class ScriptManager;
class GameObject;
class SceneManager;

void DrawInspector(GameObject *obj);

struct ComponentBase
{
    GameObject *gameObject;

    virtual void DrawInspector() {};
    friend void DrawInspector(GameObject *obj);
    SceneManager* get_sceneMgr();
};

class ScriptBase
{
public:
    PARAMS;

    virtual void Start() {};
    virtual void Update(float delta) {};
    virtual void OnDestroy() {};
    virtual void CallBack(Event::Event &event) {};

    std::string m_ScriptName;
    Event_ID m_CallID;
    GameObject *gameObject;
    friend class ScriptManager;
    friend class SceneManager;
};

struct ScriptComponent : public ComponentBase
{
    Ref<ScriptBase> m_script;

    void set_script(Ref<ScriptBase> script);
    void set_script(const std::string& scriptName);

    void Start()
    {
        if (m_script)
            m_script->Start();
    }

    void Update(float delta)
    {
        if (m_script)
            m_script->Update(delta);
    }

    void OnDestroy();
    void DrawInspector() override;
};

struct TransformComponent : public ComponentBase
{
    glm::vec3 m_position = glm::vec3(0);
    glm::vec3 m_scale = glm::vec3(1);
    glm::quat m_rot = glm::quat(glm::mat3(1));

    glm::mat4 m_model = glm::mat4(0);

    bool m_static = false;

    GameObject *m_father = nullptr;
    std::vector<GameObject *> m_children;

    void attach(TransformComponent *father);
    void detach();
    glm::vec3 get_world_pos();
    void set_world_pos(glm::vec3 pos);
    glm::vec3 get_right();
    glm::vec3 get_forward();
    glm::vec3 get_rotEuler() const { return utils::to_euler(m_rot); }
    void set_rotEuler(const glm::vec3 &degrees) { m_rot = utils::to_quat(degrees); }

    void rotate_local(const glm::vec3 &degrees)
    {
        m_rot = m_rot * utils::to_quat(degrees);
    }

    void rotate_world(const glm::vec3 &degrees)
    {
        m_rot = utils::to_quat(degrees) * m_rot;
    }

    void get_allChildren(std::vector<GameObject *> &children);
    bool have_child(GameObject *object);
    void DrawInspector() override;
};

struct MeshComponent : public ComponentBase
{
    Ref<Mesh> m_mesh;
    bool m_castShadow = true;

    void DrawInspector() override;
};

struct RendererComponent : public ComponentBase
{
    std::vector<Ref<Material>> m_materials;

    void Render();

    static void DrawParams(ParamDic &params);

    void DrawInspector() override;
};

struct PointLightComponent : public ComponentBase
{
    PointLight_t m_data;

    void DrawInspector() override;
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
    bool m_Active = false;

    void DrawInspector() override;

    glm::mat4 get_proj();
    glm::vec3 get_pos();
};