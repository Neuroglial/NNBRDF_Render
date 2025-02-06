#pragma once
#include <entt/entt.hpp>
#include "utils/utils.hpp"
#include "Scene/MeshManager.hpp"

class ScriptManager;

class ScriptBase
{
public:
    virtual void Start() = 0;
    virtual void Update(float delta) = 0;

private:
    template <typename T>
    T *GetComponent()
    {
        return m_reg->try_get<T>(m_id);
    }
    entt::entity m_id;
    entt::registry *m_reg = nullptr;
    friend class ScriptManager;
};

struct ScriptComponent
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
};

struct TransformComponent
{
    glm::vec3 m_pos = glm::vec3(0);
    glm::vec3 m_scl = glm::vec3(1);
    glm::vec3 m_rot = glm::vec3(0);

    glm::mat4 m_model = glm::mat4(1);

    entt::entity m_father{entt::null};
    std::vector<entt::entity> m_children;
};

struct MeshComponent
{
    Ref<Mesh> m_mesh;
    Ref<Material> m_mat;
};