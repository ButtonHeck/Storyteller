#pragma once

#include "pointers.h"
#include "window_application.h"
#include "window.h"
#include "editor_ui.h"

namespace Storyteller
{
    class EditorApplication : public WindowApplication
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
        Ptr<EditorUi> _ui;
    };
    //--------------------------------------------------------------------------
}
