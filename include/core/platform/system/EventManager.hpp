// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#pragma once
#include <GLFW/glfw3.h>
#include <functional>
#include <map>
#include "core/platform/system/Event.hpp"

using Event_ID = uint32_t;

class EventManager
{
public:
    void triggerEvents(Event::Event &event);

    Event_ID registerCallback(std::function<void(Event::Event &)> callback);

    void unregister(Event_ID tg);

    EventManager();

private:
    Event_ID m_tg;
    std::map<Event_ID, std::function<void(Event::Event &)>> m_callBacks;
};
