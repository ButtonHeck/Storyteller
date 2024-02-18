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
        virtual bool OnWindowMoveEvent(WindowMoveEvent& event) = 0;
        virtual bool OnWindowCloseEvent(WindowCloseEvent& event) = 0;
        virtual bool OnWindowResizeEvent(WindowResizeEvent& event) = 0;
        virtual bool OnWindowFocusEvent(WindowFocusEvent& event) = 0;
        virtual bool OnWindowIconifyEvent(WindowIconifyEvent& event) = 0;
        virtual bool OnWindowFramebufferRefreshEvent(WindowFramebufferRefreshEvent& event) = 0;
        virtual bool OnWindowFramebufferResizeEvent(WindowFramebufferResizeEvent& event) = 0;

        virtual bool OnMouseMoveEvent(MouseMoveEvent& event) = 0;
        virtual bool OnMouseScrollEvent(MouseScrollEvent& event) = 0;
        virtual bool OnMouseButtonPressEvent(MouseButtonPressEvent& event) = 0;
        virtual bool OnMouseButtonReleaseEvent(MouseButtonReleaseEvent& event) = 0;

    protected:
        Ptr<Window> _window;
    };
    //--------------------------------------------------------------------------
}