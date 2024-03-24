#pragma once

#include "editor_ui.h"
#include "Storyteller/pointers.h"
#include "Storyteller/window_application.h"
#include "Storyteller/window.h"

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

    protected:
        void OnEvent(Event& event) override;
        
        bool OnWindowCloseEvent(WindowCloseEvent& event) override;
        bool OnWindowFramebufferRefreshEvent(WindowFramebufferRefreshEvent& event) override;

        bool OnKeyPressEvent(KeyPressEvent& event) override;

    private:
        void SaveSettings() const;
        void LoadSettings();

    private:
        Ptr<EditorUi> _ui;
    };
    //--------------------------------------------------------------------------
}
