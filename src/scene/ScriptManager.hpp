#pragma once
#include <map>
#include <functional>
#include "utils/utils.hpp"
#include "scene/Component.hpp"

class ScriptManager
{
public:
    static Ref<std::map<std::string, std::function<ScriptBase *()>>> get_map()
    {
        static Ref<std::map<std::string, std::function<ScriptBase *()>>> m_creator(new std::map<std::string, std::function<ScriptBase *()>>);
        return m_creator;
    }

    static Ref<ScriptBase> create(const std::string &name, entt::registry *reg)
    {
        auto map = get_map();
        auto i = map->find(name);
        if (i != map->end())
        {
            Ref<ScriptBase> rt(i->second());
            return rt;
        }
        return nullptr;
    }
};

#define REGISTER_SCRITPT(classType)                                                     \
    struct REGIDTER_SCRIPT_##classType                                                  \
    {                                                                                   \
        REGIDTER_SCRIPT_##classType()                                                   \
        {                                                                               \
            auto map = ScriptManager::get_map();                                        \
            map->emplace(#classType, []() -> ScriptBase * { return new classType(); }); \
        }                                                                               \
    };                                                                                  \
    REGIDTER_SCRIPT_##classType REGIDTER_INSTANCE_##classType;