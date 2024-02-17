#pragma once

#include "event.h"
#include "key_codes.h"
#include "utils.h"

namespace Storyteller
{
    struct KeyEvent : public Event
    {
        EVENT_CLASS_TRAITS(KeyboardEventTrait | InputEventTrait)

        KeyCode GetKeyCode() const
        {
            return _keyCode;
        }

    protected:
        KeyEvent(const KeyCode keyCode)
            : _keyCode(keyCode)
        {}

    protected:
        KeyCode _keyCode;
    };
    //--------------------------------------------------------------------------


    struct KeyPressEvent : public KeyEvent
    {
        EVENT_CLASS_TYPE(KeyPressEventType)

        KeyPressEvent(const KeyCode keyCode, bool repeat = false)
            : KeyEvent(keyCode)
            , _repeat(repeat)
        {}

        bool IsRepeat() const
        {
            return _repeat;
        }

        std::string ToString() const override
        {
            return Utils::Concatenate("KeyPressEvent: ", _keyCode, " (repeat = ", _repeat, ")");
        }

    private:
        bool _repeat;
    };
    //--------------------------------------------------------------------------


    struct KeyReleaseEvent : public KeyEvent
    {
        EVENT_CLASS_TYPE(KeyReleaseEventType)

        KeyReleaseEvent(const KeyCode keyCode)
            : KeyEvent(keyCode)
        {}

        std::string ToString() const override
        {
            return Utils::Concatenate("KeyReleaseEvent: ", _keyCode);
        }
    };
    //--------------------------------------------------------------------------


    struct KeyCharEvent : public KeyEvent
    {
        EVENT_CLASS_TYPE(KeyCharEventType)

        KeyCharEvent(const KeyCode keyCode)
            : KeyEvent(keyCode)
        {}

        std::string ToString() const override
        {
            return Utils::Concatenate("KeyCharEvent: ", _keyCode);
        }
    };
    //--------------------------------------------------------------------------
}
