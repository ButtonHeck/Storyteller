#include "window.h"
#include "window_glfw.h"

namespace Storyteller
{
    Window* Window::StrtlrCreateWindow()
    {
#ifdef STRTLR_WINDOW_BACKEND_GLFW
        return new WindowGlfw();
#else
#error "No window backend is provided!"
#endif
        return nullptr;
    }
    //--------------------------------------------------------------------------
}