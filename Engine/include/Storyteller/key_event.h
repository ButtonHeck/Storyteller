#pragma once

#include "event.h"
#include "key_codes.h"
#include "utils.h"

namespace Storyteller
{
    struct KeyEvent : public Event
    {
        int GetTraits() const override
        {
            return KeyboardEventTrait | InputEventTrait;
        }

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

        KeyPressEvent(const KeyCode keyCode, int mods, bool repeat = false)
            : KeyEvent(keyCode)
            , _mods(mods)
            , _repeat(repeat)
        {}

        bool IsRepeat() const
        {
            return _repeat;
        }

        int GetMods() const
        {
            return _mods;
        }

        std::string ToString() const override
        {
            return Utils::Concatenate("KeyPressEvent: ", _keyCode, " (repeat = ", _repeat, ", mods = ", _mods, ")");
        }

    private:
        int _mods;
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
