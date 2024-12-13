// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#include "core/platform/system/EventManager.hpp"

void EventManager::triggerEvents(Event::Event& event){
    for(auto callback:m_callBacks){
        callback.second(event);
        if(event.m_done)break;
    }
}


int EventManager::registerCallback(std::function<void(Event::Event&)> callback){
    m_callBacks.insert(std::pair<int,std::function<void(Event::Event&)>>(m_tg,callback));
    return m_tg++;
}


void EventManager::unregister(int tg){
    m_callBacks.erase(tg);
}


EventManager::EventManager():m_tg(0){}