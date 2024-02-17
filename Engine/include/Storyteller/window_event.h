#pragma once

#include "event.h"
#include "utils.h"

namespace Storyteller
{
    struct WindowEvent : public Event
    {
        EVENT_CLASS_TRAITS(WindowEventTrait)

    protected:
        WindowEvent() = default;
    };
    //--------------------------------------------------------------------------


    struct WindowResizeEvent : public WindowEvent
    {
        EVENT_CLASS_TYPE(WindowResizeEventType)

        WindowResizeEvent(unsigned int width, unsigned int height)
            : _width(width)
            , _height(height)
        {}

        unsigned int GetWidth() const
        {
            return _width;
        }

        unsigned int GetHeight() const
        {
            return _height;
        }

        std::string ToString() const override
        {
            return Utils::Concatenate("WindowResizeEvent: ", _width, ", ", _height);
        }

    private:
        unsigned int _width;
        unsigned int _height;
    };
    //--------------------------------------------------------------------------


    struct WindowMoveEvent : public WindowEvent
    {
        EVENT_CLASS_TYPE(WindowMoveEventType)

        WindowMoveEvent(unsigned int x, unsigned int y)
            : _x(x)
            , _y(y)
        {}

        unsigned int GetX() const
        {
            return _x;
        }

        unsigned int GetY() const
        {
            return _y;
        }

        std::string ToString() const override
        {
            return Utils::Concatenate("WindowMoveEvent: ", _x, ", ", _y);
        }

    private:
        unsigned int _x;
        unsigned int _y;
    };
    //--------------------------------------------------------------------------


    struct WindowCloseEvent : public WindowEvent
    {
        EVENT_CLASS_TYPE(WindowCloseEventType)

        WindowCloseEvent() = default;
    };
    //--------------------------------------------------------------------------


    struct WindowFocusEvent : public WindowEvent
    {
        EVENT_CLASS_TYPE(WindowFocusEventType)

        WindowFocusEvent(int focused)
            : _focused(focused)
        {}

        bool IsFocused() const
        {
            return bool(_focused);
        }

        std::string ToString() const override
        {
            return Utils::Concatenate("WindowFocusEvent: ", IsFocused());
        }

    private:
        int _focused;
    };
    //--------------------------------------------------------------------------


    struct WindowIconifyEvent : public WindowEvent
    {
        EVENT_CLASS_TYPE(WindowIconifyEventType)

        WindowIconifyEvent(int iconified)
            : _iconified(iconified)
        {}

        bool IsIconified() const
        {
            return bool(_iconified);
        }

        std::string ToString() const override
        {
            return Utils::Concatenate("WindowIconifyEvent: ", IsIconified());
        }

    private:
        int _iconified;
    };
    //--------------------------------------------------------------------------


    struct WindowFramebufferResizeEvent : public WindowEvent
    {
        EVENT_CLASS_TYPE(WindowFramebufferResizeEventType)

        WindowFramebufferResizeEvent(unsigned int width, unsigned int height)
            : _width(width)
            , _height(height)
        {}

        unsigned int GetWidth() const
        {
            return _width;
        }

        unsigned int GetHeight() const
        {
            return _height;
        }

        std::string ToString() const override
        {
            return Utils::Concatenate("WindowFramebufferResizeEvent: ", _width, ", ", _height);
        }

    private:
        unsigned int _width;
        unsigned int _height;
    };
    //--------------------------------------------------------------------------


    struct WindowFramebufferRefreshEvent : public WindowEvent
    {
        EVENT_CLASS_TYPE(WindowFramebufferRefreshEventType)

        WindowFramebufferRefreshEvent() = default;
    };
    //--------------------------------------------------------------------------
}
