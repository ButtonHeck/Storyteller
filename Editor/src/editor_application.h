#pragma once

#include "application.h"
#include "window.h"
#include "editor_ui.h"

namespace Storyteller
{
    class EditorApplication : public Application
    {
    public:
        EditorApplication();

        std::string GetApplicationName() const override;

        bool Initialize() override;
        void Run() override;

    private:
        Window::Ptr _window;
        EditorUi::Ptr _ui;
    };
    //--------------------------------------------------------------------------
}
