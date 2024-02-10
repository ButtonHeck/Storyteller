#pragma once

#include "Storyteller/window.h"

namespace Storyteller
{
    class EditorUiImplOpenglGLFW
    {
    public:
        explicit EditorUiImplOpenglGLFW(Ptr<Window> window);

        void Initialize();
        void NewFrame();
        void Render();
        void Shutdown();

    private:
        Ptr<Window> _window;
    };
    //--------------------------------------------------------------------------
}