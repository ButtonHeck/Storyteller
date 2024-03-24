#pragma once

#include "pointers.h"
#include "application.h"
#include "window.h"
#include "window_event.h"
#include "mouse_event.h"

namespace Storyteller
{
    class WindowApplication : public Application
    {
    public:
        WindowApplication();

        bool Initialize() override;

    protected:
        virtual bool OnWindowMoveEvent(WindowMoveEvent& event) { return true; };
        virtual bool OnWindowCloseEvent(WindowCloseEvent& event) { return true; };
        virtual bool OnWindowResizeEvent(WindowResizeEvent& event) { return true; };
        virtual bool OnWindowFocusEvent(WindowFocusEvent& event) { return true; };
        virtual bool OnWindowIconifyEvent(WindowIconifyEvent& event) { return true; };
        virtual bool OnWindowFramebufferRefreshEvent(WindowFramebufferRefreshEvent& event) { return true; };
        virtual bool OnWindowFramebufferResizeEvent(WindowFramebufferResizeEvent& event) { return true; };

        virtual bool OnMouseMoveEvent(MouseMoveEvent& event) { return true; };
        virtual bool OnMouseScrollEvent(MouseScrollEvent& event) { return true; };
        virtual bool OnMouseButtonPressEvent(MouseButtonPressEvent& event) { return true; };
        virtual bool OnMouseButtonReleaseEvent(MouseButtonReleaseEvent& event) { return true; };

    protected:
        Ptr<Window> _window;
    };
    //--------------------------------------------------------------------------
}