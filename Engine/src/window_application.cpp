#include "window_application.h"

namespace Storyteller
{
    WindowApplication::WindowApplication()
        : _window(nullptr)
    {}
    //--------------------------------------------------------------------------

    bool WindowApplication::Initialize(const std::string& configPath)
    {
        if (!Application::Initialize(configPath))
        {
            return false;
        }

        _window.reset(Window::Create());
        if (!_window || !_window->Initialize())
        {
            return false;
        }

        return true;
    }
    //--------------------------------------------------------------------------
}