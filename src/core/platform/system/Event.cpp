#include "core/platform/system/Event.hpp"



namespace std{
    #define TO_STRING(x) case x: return #x;
    std::string to_string(KeyCode key){
        switch (key)
        {
        TO_STRING(KeyCode::Space)
        TO_STRING(KeyCode::Apostrophe)
        TO_STRING(KeyCode::Comma)
        TO_STRING(KeyCode::Minus)
        TO_STRING(KeyCode::Period)
        TO_STRING(KeyCode::Slash)

        TO_STRING(KeyCode::D1)
        TO_STRING(KeyCode::D2)
        TO_STRING(KeyCode::D3)
        TO_STRING(KeyCode::D4)
        TO_STRING(KeyCode::D5)
        TO_STRING(KeyCode::D6)
        TO_STRING(KeyCode::D7)
        TO_STRING(KeyCode::D8)
        TO_STRING(KeyCode::D9)

        TO_STRING(KeyCode::Semicolon)
        TO_STRING(KeyCode::Equal)

        TO_STRING(KeyCode::A)
        TO_STRING(KeyCode::B)
        TO_STRING(KeyCode::C)
        TO_STRING(KeyCode::D)
        TO_STRING(KeyCode::E)
        TO_STRING(KeyCode::F)
        TO_STRING(KeyCode::G)
        TO_STRING(KeyCode::H)
        TO_STRING(KeyCode::I)
        TO_STRING(KeyCode::J)
        TO_STRING(KeyCode::K)
        TO_STRING(KeyCode::L)
        TO_STRING(KeyCode::M)
        TO_STRING(KeyCode::N)
        TO_STRING(KeyCode::O)
        TO_STRING(KeyCode::P)
        TO_STRING(KeyCode::Q)
        TO_STRING(KeyCode::R)
        TO_STRING(KeyCode::S)
        TO_STRING(KeyCode::T)
        TO_STRING(KeyCode::U)
        TO_STRING(KeyCode::V)
        TO_STRING(KeyCode::W)
        TO_STRING(KeyCode::X)
        TO_STRING(KeyCode::Y)
        TO_STRING(KeyCode::Z)

        TO_STRING(KeyCode::LeftBracket)
        TO_STRING(KeyCode::Backslash)
        TO_STRING(KeyCode::RightBracket)
        TO_STRING(KeyCode::GraveAccent)

        TO_STRING(KeyCode::World1)
        TO_STRING(KeyCode::World2)

        TO_STRING(KeyCode::Escape)
        TO_STRING(KeyCode::Enter)
        TO_STRING(KeyCode::Tab)
        TO_STRING(KeyCode::Backspace)
        TO_STRING(KeyCode::Insert)
        TO_STRING(KeyCode::Delete)
        TO_STRING(KeyCode::Right)
        TO_STRING(KeyCode::Left)
        TO_STRING(KeyCode::Down)
        TO_STRING(KeyCode::Up)
        TO_STRING(KeyCode::PageUp)
        TO_STRING(KeyCode::PageDown)
        TO_STRING(KeyCode::Home)
        TO_STRING(KeyCode::End)
        TO_STRING(KeyCode::CapsLock)
        TO_STRING(KeyCode::ScrollLock)
        TO_STRING(KeyCode::NumLock)
        TO_STRING(KeyCode::PrintScreen)
        TO_STRING(KeyCode::Pause)

        TO_STRING(KeyCode::F1)
        TO_STRING(KeyCode::F2)
        TO_STRING(KeyCode::F3)
        TO_STRING(KeyCode::F4)
        TO_STRING(KeyCode::F5)
        TO_STRING(KeyCode::F6)
        TO_STRING(KeyCode::F7)
        TO_STRING(KeyCode::F8)
        TO_STRING(KeyCode::F9)
        TO_STRING(KeyCode::F10)
        TO_STRING(KeyCode::F11)
        TO_STRING(KeyCode::F12)
        TO_STRING(KeyCode::F13)
        TO_STRING(KeyCode::F14)
        TO_STRING(KeyCode::F15)
        TO_STRING(KeyCode::F16)
        TO_STRING(KeyCode::F17)
        TO_STRING(KeyCode::F18)
        TO_STRING(KeyCode::F19)
        TO_STRING(KeyCode::F20)
        TO_STRING(KeyCode::F21)
        TO_STRING(KeyCode::F22)
        TO_STRING(KeyCode::F23)
        TO_STRING(KeyCode::F24)
        TO_STRING(KeyCode::F25)

        TO_STRING(KeyCode::KP0)
        TO_STRING(KeyCode::KP1)
        TO_STRING(KeyCode::KP2)
        TO_STRING(KeyCode::KP3)
        TO_STRING(KeyCode::KP4)
        TO_STRING(KeyCode::KP5)
        TO_STRING(KeyCode::KP6)
        TO_STRING(KeyCode::KP7)
        TO_STRING(KeyCode::KP8)
        TO_STRING(KeyCode::KP9)

        TO_STRING(KeyCode::KPDecimal)
        TO_STRING(KeyCode::KPDivide)
        TO_STRING(KeyCode::KPMultiply)
        TO_STRING(KeyCode::KPSubtract)
        TO_STRING(KeyCode::KPAdd)
        TO_STRING(KeyCode::KPEnter)
        TO_STRING(KeyCode::KPEqual)

        TO_STRING(KeyCode::LeftShift)
        TO_STRING(KeyCode::LeftControl)
        TO_STRING(KeyCode::LeftAlt)
        TO_STRING(KeyCode::LeftSuper)
        TO_STRING(KeyCode::RightShift)
        TO_STRING(KeyCode::RightControl)
        TO_STRING(KeyCode::RightAlt)
        TO_STRING(KeyCode::RightSuper)
        TO_STRING(KeyCode::Menu)

        default: return "KeyCode::None";
        }
    }
    std::string to_string(PressType pt){
        switch (pt)
        {
        TO_STRING(PressType::None)
        TO_STRING(PressType::Press)
        TO_STRING(PressType::Release)
        TO_STRING(PressType::Repeat)
        default: return "PressType::None";
        }
    }
    std::string to_string(PressMode mode){
        switch (mode)
        {
        TO_STRING(PressMode::WithNone)
        TO_STRING(PressMode::WithShift)
        TO_STRING(PressMode::WithControl)
        TO_STRING(PressMode::WithAlt)
        TO_STRING(PressMode::WithSuper)
        default: return "PressMode::None";
        }
    }
}