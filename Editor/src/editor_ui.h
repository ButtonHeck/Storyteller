#pragma once

#include "window.h"

#include <memory>

namespace Storyteller
{
    class EditorUi
    {
    public:
        typedef std::shared_ptr<EditorUi> Ptr;

        EditorUi(Window::Ptr window);

        bool Initialize();
        void Prepare();
        void Compose();
        void Render();

    private:
        Window::Ptr _window;
    };
    //--------------------------------------------------------------------------
}
