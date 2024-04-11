#pragma once

#include "Storyteller/pointers.h"
#include "Storyteller/window.h"

namespace Storyteller
{
    class EditorUiImpl
    {
    public:
        static EditorUiImpl* CreateImpl(const Ptr<Window> window);

    public:
        explicit EditorUiImpl(const Ptr<Window> window);
        virtual ~EditorUiImpl() = default;

        virtual void Initialize() = 0;
        virtual void NewFrame() = 0;
        virtual void Render() = 0;
        virtual void Shutdown() = 0;

    protected:
        const Ptr<Window> _window;
    };
    //--------------------------------------------------------------------------
}
