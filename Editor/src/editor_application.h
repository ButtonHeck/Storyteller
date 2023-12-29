#pragma once

#include "application.h"
#include "window.h"

namespace Storyteller
{
    class EditorApplication : public Application
    {
    public:
        EditorApplication();

        virtual ~EditorApplication() = default;

        bool Initialize() override;
        void Run() override;

    private:
        Window::Ptr _window;
    };
    //--------------------------------------------------------------------------
}
