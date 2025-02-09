#pragma once
#include <entt/entt.hpp>
#include "utils/utils.hpp"
#include "Scene/MeshManager.hpp"

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
            script->OnDestroy();
    }
};

struct TransformComponent : public ComponentBase
{
    glm::vec3 m_pos = glm::vec3(0);
    glm::vec3 m_scl = glm::vec3(1);
    glm::vec3 m_rot = glm::vec3(0);

    glm::mat4 m_model = glm::mat4(0);

    bool m_static = true;

    GameObject *m_father = nullptr;
    std::vector<GameObject *> m_children;

    void attach(TransformComponent *father);
    void detach();
    glm::vec3 get_world_pos();
    glm::vec3 get_right();
    glm::vec3 get_forword();
};

struct MeshComponent : public ComponentBase
{
    Ref<Mesh> m_mesh;
};

struct RendererComponent : public ComponentBase
{
    std::vector<Ref<Material>> m_materials;
};