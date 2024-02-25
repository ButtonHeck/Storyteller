#pragma once

#include "editor_ui_impl.h"
#include "Storyteller/window.h"

namespace Storyteller
{
    class EditorUiImplOpenglGLFW : public EditorUiImpl
    {
    public:
        explicit EditorUiImplOpenglGLFW(Ptr<Window> window);

        void Initialize() override;
        void NewFrame() override;
        void Render() override;
        void Shutdown() override;
    };
    //--------------------------------------------------------------------------
}