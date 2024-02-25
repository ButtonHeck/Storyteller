#pragma once

#include "Storyteller/pointers.h"
#include "Storyteller/window.h"

namespace Storyteller
{
    class EditorUiImpl
    {
    public:
        static EditorUiImpl* CreateImpl(Ptr<Window> window);

    public:
        explicit EditorUiImpl(Ptr<Window> window);

        virtual void Initialize() = 0;
        virtual void NewFrame() = 0;
        virtual void Render() = 0;
        virtual void Shutdown() = 0;

    protected:
        Ptr<Window> _window;
    };
    //--------------------------------------------------------------------------
}