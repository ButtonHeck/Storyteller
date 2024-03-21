#include "window_application.h"

namespace Storyteller
{
    WindowApplication::WindowApplication()
        : _window(nullptr)
    {}
    //--------------------------------------------------------------------------

    bool WindowApplication::Initialize()
    {
        if (!Application::Initialize())
        {
            return false;
        }

        _window.reset(Window::StrtlrCreateWindow());
        if (!_window || !_window->Initialize())
        {
            return false;
        }

        return true;
    }
    //--------------------------------------------------------------------------
}