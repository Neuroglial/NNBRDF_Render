// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#pragma once
#include <GLFW/glfw3.h>
#include <functional>
#include <map>
#include "core/platform/system/Event.hpp"


class EventManager{
    public:

    void triggerEvents(Event::Event& event);

    int registerCallback(std::function<void(Event::Event&)> callback);

    void unregister(int tg);

    EventManager();

    private:
    int m_tg;
    std::map<int,std::function<void(Event::Event&)>> m_callBacks;
};
