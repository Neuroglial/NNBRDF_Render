#include "core/platform/system/EventManager.hpp"

EventManage EventManage::instance;

void EventManage::triggerEvents(Event::Base& event){
    for(auto callback:m_callBacks){
        callback.second(event);
        if(event.m_done)break;
    }
}


int EventManage::registerCallback(std::function<void(Event::Base&)> callback){
    m_callBacks.insert(std::pair(m_tg,callback));
    return m_tg++;
}


void EventManage::unregister(int tg){
    m_callBacks.erase(tg);
}


EventManage::EventManage():m_tg(0){}