#pragma once
#include <string>

#define EVENT_IF(type, dir_name, src_name)                  \
    if (src_name.get_type() != type::get_type_static())     \
        return;                                             \
    type &dir_name = *static_cast<type *>(&src_name);

#define EVENT_CLASS_TYPE(type)                                      \
    static Type get_type_static() { return type; }                  \
    virtual Type get_type() override { return get_type_static(); }  \
    virtual std::string get_name() override { return #type; }

namespace Event
{

    enum Type
    {
        NONE,
        FRAME_RESIZE,
    };

    struct Event
    {
        bool m_done = false;
        virtual Type get_type() = 0;
        virtual std::string get_event() = 0;
        virtual std::string get_name() = 0;
    };

    struct Event_Frame_Resize : public Event
    {
        EVENT_CLASS_TYPE(FRAME_RESIZE)

        virtual std::string get_event() override
        {
            return "FrameBufferResize: Width(" + std::to_string(m_width) + ") Height(" + std::to_string(m_height) + ")\n";
        }

        Event_Frame_Resize() = delete;

        Event_Frame_Resize(int width, int height) : m_width(width), m_height(height) {}

        const int m_width, m_height;
    };

}