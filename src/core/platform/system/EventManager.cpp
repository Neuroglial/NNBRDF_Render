// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#include "core/platform/system/EventManager.hpp"
#include "utils/utils.hpp"

void EventManager::triggerEvents(Event::Event &event)
{
    for (auto& i : m_callBacks)
    {
        i.second(event);
        if (event.m_done)
            break;
    }
}

Event_ID EventManager::registerCallback(std::function<void(Event::Event &)> callback)
{
    m_callBacks.insert(std::pair<int, std::function<void(Event::Event &)>>(m_tg, callback));
    return m_tg++;
}

Event_ID EventManager::registerDistributor(EventManager &distributor)
{
    auto *disPtr = &distributor;
    Assert(disPtr != this);
    return registerCallback(std::bind(&EventManager::triggerEvents, disPtr, std::placeholders::_1));
}

void EventManager::deleteCallback(Event_ID tg)
{
    m_callBacks.erase(tg);
}

EventManager::EventManager() : m_tg(0) {}