// Copyright (c) 2024 [Neruo]
// This program is licensed under the terms of the GPL-3.0 License.
// See the LICENSE file for more details.

#pragma once
#include <string>

#define EVENT_IF(type, dir_name, src_name)              \
    if (src_name.get_type() != type::get_type_static()) \
        return;                                         \
    type &dir_name = *(type *)(&src_name);

#define EVENT_CLASS_TYPE(type)                                     \
    static Type get_type_static() { return type; }                 \
    virtual Type get_type() override { return get_type_static(); } \
    virtual std::string get_name() override { return #type; }

enum class KeyCode
{
    MouseLeft = 0,
    MouseRight = 1,
    MouseMiddle = 2,
    MouseButton4 = 3,
    MouseButton5 = 4,
    MouseButton6 = 6,
    MouseButton7 = 7,
    MouseButton8 = 8,
    
    // From glfw3.h
    Space = 32,
    Apostrophe = 39, /* ' */
    Comma = 44,      /* , */
    Minus = 45,      /* - */
    Period = 46,     /* . */
    Slash = 47,      /* / */

    D0 = 48, /* 0 */
    D1 = 49, /* 1 */
    D2 = 50, /* 2 */
    D3 = 51, /* 3 */
    D4 = 52, /* 4 */
    D5 = 53, /* 5 */
    D6 = 54, /* 6 */
    D7 = 55, /* 7 */
    D8 = 56, /* 8 */
    D9 = 57, /* 9 */

    Semicolon = 59, /* ; */
    Equal = 61,     /* = */

    A = 65,
    B = 66,
    C = 67,
    D = 68,
    E = 69,
    F = 70,
    G = 71,
    H = 72,
    I = 73,
    J = 74,
    K = 75,
    L = 76,
    M = 77,
    N = 78,
    O = 79,
    P = 80,
    Q = 81,
    R = 82,
    S = 83,
    T = 84,
    U = 85,
    V = 86,
    W = 87,
    X = 88,
    Y = 89,
    Z = 90,

    LeftBracket = 91,  /* [ */
    Backslash = 92,    /* \ */
    RightBracket = 93, /* ] */
    GraveAccent = 96,  /* ` */

    World1 = 161, /* non-US #1 */
    World2 = 162, /* non-US #2 */

    /* Function keys */
    Escape = 256,
    Enter = 257,
    Tab = 258,
    Backspace = 259,
    Insert = 260,
    Delete = 261,
    Right = 262,
    Left = 263,
    Down = 264,
    Up = 265,
    PageUp = 266,
    PageDown = 267,
    Home = 268,
    End = 269,
    CapsLock = 280,
    ScrollLock = 281,
    NumLock = 282,
    PrintScreen = 283,
    Pause = 284,
    F1 = 290,
    F2 = 291,
    F3 = 292,
    F4 = 293,
    F5 = 294,
    F6 = 295,
    F7 = 296,
    F8 = 297,
    F9 = 298,
    F10 = 299,
    F11 = 300,
    F12 = 301,
    F13 = 302,
    F14 = 303,
    F15 = 304,
    F16 = 305,
    F17 = 306,
    F18 = 307,
    F19 = 308,
    F20 = 309,
    F21 = 310,
    F22 = 311,
    F23 = 312,
    F24 = 313,
    F25 = 314,

    /* Keypad */
    KP0 = 320,
    KP1 = 321,
    KP2 = 322,
    KP3 = 323,
    KP4 = 324,
    KP5 = 325,
    KP6 = 326,
    KP7 = 327,
    KP8 = 328,
    KP9 = 329,
    KPDecimal = 330,
    KPDivide = 331,
    KPMultiply = 332,
    KPSubtract = 333,
    KPAdd = 334,
    KPEnter = 335,
    KPEqual = 336,

    LeftShift = 340,
    LeftControl = 341,
    LeftAlt = 342,
    LeftSuper = 343,
    RightShift = 344,
    RightControl = 345,
    RightAlt = 346,
    RightSuper = 347,
    Menu = 348,

    None,
};

enum class PressType
{
    Release = 0,
    Press = 1,
    Repeat = 2,
    None,
};

enum class PressMode
{
    WithNone = 0x0000,
    WithShift = 0x0001,
    WithControl = 0x0002,
    WithAlt = 0x0004,
    WithSuper = 0x0008,
    WithCapsLock = 0x0010,
    WithNumLock = 0x0020,
};

namespace std
{
    std::string to_string(KeyCode key);
    std::string to_string(PressType pt);
    std::string to_string(PressMode mode);
}

namespace Event
{

    enum Type
    {
        NONE,
        FRAME_RESIZE,
        KEYBOARD_INPUT,
        MOUSE_MOVE,
        SCROLL,
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
            return "FrameBufferResize: Width(" + std::to_string(m_width) + ") Height(" + std::to_string(m_height) + ")";
        }

        Event_Frame_Resize() = delete;

        Event_Frame_Resize(int width, int height) : m_width(width), m_height(height) {}

        const int m_width, m_height;
    };

    struct Event_Keyboard : public Event
    {
        EVENT_CLASS_TYPE(KEYBOARD_INPUT)

        virtual std::string get_event() override
        {
            std::string re = "KeyboardInput: " + std::to_string(m_pressType) + " " + std::to_string(m_code);
            if(m_mode&(int)PressMode::WithAlt){
                re += " " + std::to_string(PressMode::WithAlt);
            }
            if(m_mode&(int)PressMode::WithCapsLock){
                re += " " + std::to_string(PressMode::WithCapsLock);
            }
            if(m_mode&(int)PressMode::WithControl){
                re += " " + std::to_string(PressMode::WithControl);
            }
            if(m_mode&(int)PressMode::WithNumLock){
                re += " " + std::to_string(PressMode::WithNumLock);
            }
            if(m_mode&(int)PressMode::WithShift){
                re += " " + std::to_string(PressMode::WithShift);
            }
            if(m_mode&(int)PressMode::WithSuper){
                re += " " + std::to_string(PressMode::WithSuper);
            }
            return  re;
        }

        Event_Keyboard() = delete;

        Event_Keyboard(KeyCode code, PressType type, int mode) : m_code(code), m_pressType(type), m_mode(mode) {}

        KeyCode m_code;
        PressType m_pressType;

        // PressMode bitMask
        int m_mode;
    };

    struct Event_Mouse_Move : public Event
    {
        EVENT_CLASS_TYPE(MOUSE_MOVE)

        virtual std::string get_event() override
        {
            return "Mouse Move: xpos(" + std::to_string(m_xpos) + ") ypos(" + std::to_string(m_ypos) + ")";
        }

        Event_Mouse_Move() = delete;

        Event_Mouse_Move(double xpos, double ypos) : m_xpos(xpos), m_ypos(ypos) {}

        const double m_xpos, m_ypos;
    };

    struct Event_Scroll : public Event
    {
        EVENT_CLASS_TYPE(SCROLL)

        virtual std::string get_event() override
        {
            return "Scroll: xoffset(" + std::to_string(m_xoffset) + ") yoffset(" + std::to_string(m_yoffset) + ")";
        }

        Event_Scroll() = delete;

        Event_Scroll(double xoffset, double yoffset) : m_xoffset(xoffset), m_yoffset(yoffset) {}

        const double m_xoffset, m_yoffset;
    };

}