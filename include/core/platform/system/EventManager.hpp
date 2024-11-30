#pragma once
#include <GLFW/glfw3.h>
#include <functional>
#include <map>
#include "core/platform/system/Event.hpp"


class EventManage{
    public:

    void triggerEvents(Event::Base& event);

    int registerCallback(std::function<void(Event::Base&)> callback);

    void unregister(int tg);

    EventManage();

    static EventManage instance;

    private:
    int m_tg;
    std::map<int,std::function<void(Event::Base&)>> m_callBacks;
};
