#include "editor_ui_impl.h"
#include "editor_ui_impl_opengl_glfw.h"

namespace Storyteller
{
    EditorUiImpl* EditorUiImpl::CreateImpl(const Ptr<Window> window)
    {
#if defined(STRTLR_WINDOW_BACKEND_GLFW) && defined(STRTLR_GRAPHICS_BACKEND_OPENGL)
        return new EditorUiImplOpenglGLFW(window);
#else
#error "No suitable window/graphics backends are provided!"
#endif
        return nullptr;
    }
    //--------------------------------------------------------------------------

    EditorUiImpl::EditorUiImpl(const Ptr<Window> window)
        : _window(window)
    {}
    //--------------------------------------------------------------------------
}
