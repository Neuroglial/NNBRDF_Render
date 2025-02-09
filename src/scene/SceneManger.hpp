#include <entt/entt.hpp>
#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include "utils/utils.hpp"
#include "scene/GameObject.hpp"
#include "scene/ScriptManager.hpp"
#include "scene/Component.hpp"

class Scene_Node
{
private:
    glm::vec3 m_pos = glm::vec3(0);
    glm::vec3 m_scl = glm::vec3(1);
    glm::vec3 m_rot = glm::vec3(0);

    glm::mat4 m_model = glm::mat4(1);
    bool m_change = false;
    Scene_Node *m_father = nullptr;
    std::vector<Scene_Node *> m_children;

    void add_child(Scene_Node *child)
    {
        m_children.push_back(child);
    }

    void remove_child(Scene_Node *child)
    {
        for (int i = 0; i < m_children.size(); ++i)
        {
            if (m_children[i] == child)
            {
                m_children[i] = m_children.back();
                m_children.resize(m_children.size() - 1);
                break;
            }
        }
    }

public:
    glm::vec3 get_loc_pos()
    {
        return m_pos;
    }

    glm::vec3 get_loc_scl()
    {
        return m_scl;
    }

    glm::vec3 get_loc_rot()
    {
        return m_rot;
    }

    glm::mat4 get_loc_model()
    {
        if (m_change)
            m_model = utils::get_model(m_pos, m_scl, m_rot);
        m_change = false;

        return m_model;
    }

    void set_loc_pos(const glm::vec3 &pos)
    {
        m_change = true;
        m_pos = pos;
    }

    void set_loc_scl(const glm::vec3 &scl)
    {
        m_change = true;
        m_scl = scl;
    }

    void set_loc_rot(const glm::vec3 &rot)
    {
        m_change = true;
        m_rot = rot;
    }

    glm::vec3 get_world_pos()
    {
        if (m_father)
        {
            glm::vec4 pos(m_pos, 1);
            pos = m_father->get_world_model() * pos;
            pos /= pos[3];
            return pos;
        }
        return m_pos;
    }

    glm::mat4 get_world_model()
    {
        if (m_father)
            return m_father->get_world_model() * get_loc_model();

        return get_loc_model();
    }

    void attach(Scene_Node &father)
    {
        m_father = &father;
        father.add_child(this);
    }

    void detach()
    {
        if (!m_father)
            return;

        m_father->remove_child(this);
        m_father = nullptr;
    }

    ~Scene_Node()
    {
        if (m_father)
        {
            for (auto i : m_children)
            {
                i->attach(*m_father);
            }

            detach();
        }
        else
        {
            for (auto i : m_children)
            {
                i->detach();
            }
        }
    }
};

template <typename T, typename... Args>
T &add_component_rt(entt::entity id, entt::registry *reg, Args &&...rest)
{
    return reg->emplace<T>(id, rest...);
};

template <>
ScriptComponent &add_component_rt<ScriptComponent, std::string>(entt::entity id, entt::registry *reg, std::string &&name)
{
    auto &ret = reg->emplace<ScriptComponent>(id);
    ret.script = ScriptManager::create(name, id, reg);
    return ret;
};

class SceneManager
{
public:
    Ref<GameObject> create_Object()
    {
        auto id = m_registry.create();
        auto obj = std::make_shared<GameObject>(id);
        add_component<TransformComponent>(obj);
        m_objects.push_back(obj);
        return obj;
    }

    template <typename T, typename... Args>
    T &add_component(Ref<GameObject> obj, Args &&...rest)
    {
        return add_component_rt<T>(obj->get_id(), &m_registry, std::forward<Args>(rest)...);
    }

    template <typename T>
    T *get_component(Ref<GameObject> obj)
    {
        return m_registry.try_get<TransformComponent>(obj->get_id());
    }

    bool attach(Ref<GameObject> father, Ref<GameObject> child)
    {
        auto *ftf = get_component<TransformComponent>(father);
        auto *ctf = get_component<TransformComponent>(child);

        if (ftf == nullptr || ctf == nullptr)
        {
            return false;
        }

        ftf->m_children.push_back(child->get_id());
        ctf->m_father = father->get_id();

        return true;
    }

    bool detach(Ref<GameObject> father, Ref<GameObject> child)
    {
        auto *ftf = get_component<TransformComponent>(father);
        auto *ctf = get_component<TransformComponent>(child);

        if (ftf == nullptr || ctf == nullptr)
        {
            return false;
        }

        for (int i = 0; i < ftf->m_children.size(); ++i)
        {
            if (ftf->m_children[i] == child->get_id())
            {
                ftf->m_children[i] = ftf->m_children.back();
                ftf->m_children.pop_back();
                break;
            }
        }
        ctf->m_father = entt::null;

        return true;
    }

    void Update(float delta)
    {
        update_script(delta);
        update_transform();
    }

    void Start()
    {
        m_registry.view<ScriptComponent>().each(
            [](ScriptComponent &sc)
            {
                if (sc.script)
                    sc.script->Start();
            });
    }

    entt::registry *get_reg()
    {
        return &m_registry;
    }

    void render_mesh()
    {

    };

private:
    void update_script(float delta)
    {
        m_registry.view<ScriptComponent>().each(
            [delta](ScriptComponent &sc)
            {
                if (sc.script)
                    sc.script->Update(delta);
            });
    }

    void update_transform()
    {
        std::function<void(entt::entity, glm::mat4 &)> dp;
        dp = [this, &dp](entt::entity et, glm::mat4 &father_trans)
        {
            auto &trans = m_registry.get<TransformComponent>(et);
            trans.m_model = utils::get_model(trans.m_pos, trans.m_scl, trans.m_rot, father_trans);
            for (auto &i : trans.m_children)
            {
                dp(i, trans.m_model);
            }
        };

        m_registry.view<TransformComponent>().each(
            [&dp](entt::entity entity, TransformComponent &trans)
            {
                if (trans.m_father == entt::null)
                {
                    dp(entity, glm::mat4(1));
                }
            });
    };

private:
    entt::registry m_registry;
    std::vector<Ref<GameObject>> m_objects;
};