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
        ~EditorApplication();

        std::string GetApplicationName() const override;

        bool Initialize() override;
        void Run() override;

    private:
        void SaveSettings() const;
        void LoadSettings();

    private:
        Window::Ptr _window;
        EditorUi::Ptr _ui;
    };
    //--------------------------------------------------------------------------
}
