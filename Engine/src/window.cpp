#include "window.h"
#include "window_glfw.h"

namespace Storyteller
{
    // TODO: implementation based on variables
    Window* Window::CreateWindow()
    {
        return new WindowGlfw();
    }
    //--------------------------------------------------------------------------
}