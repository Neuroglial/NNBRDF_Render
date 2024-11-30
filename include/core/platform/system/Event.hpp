#pragma once
#include <string>

namespace Event{


    enum Type{
        NONE,
        FRAMEBUFFER_RESIZE,
    };


    struct Base{
        bool m_done = false;
        virtual Type get_type() = 0;
        virtual std::string get_type_str() = 0;
    };


    struct FramebufferResize:public Base
    {
        virtual Type get_type() override{
            return Type::FRAMEBUFFER_RESIZE;
        }

        virtual std::string get_type_str() override{
            return "FrameBufferResize: Width(" + std::to_string(m_width) + ") Height(" + std::to_string(m_height) + ")\n";
        }

        FramebufferResize() = delete;

        FramebufferResize(int width,int height):m_width(width),m_height(height){}

        const int m_width,m_height;
    };
    



}