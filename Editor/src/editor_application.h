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
        bool OnWindowMoveEvent(WindowMoveEvent& event) override;
        bool OnWindowCloseEvent(WindowCloseEvent& event) override;
        bool OnWindowResizeEvent(WindowResizeEvent& event) override;
        bool OnWindowFocusEvent(WindowFocusEvent& event) override;
        bool OnWindowIconifyEvent(WindowIconifyEvent& event) override;
        bool OnWindowFramebufferRefreshEvent(WindowFramebufferRefreshEvent& event) override;
        bool OnWindowFramebufferResizeEvent(WindowFramebufferResizeEvent& event) override;

        bool OnMouseMoveEvent(MouseMoveEvent& event) override;
        bool OnMouseScrollEvent(MouseScrollEvent& event) override;
        bool OnMouseButtonPressEvent(MouseButtonPressEvent& event) override;
        bool OnMouseButtonReleaseEvent(MouseButtonReleaseEvent& event) override;

        bool OnKeyPressEvent(KeyPressEvent& event) override;
        bool OnKeyReleaseEvent(KeyReleaseEvent& event) override;
        bool OnKeyCharEvent(KeyCharEvent& event) override;

    private:
        void SaveSettings() const;
        void LoadSettings();

    private:
        Ptr<EditorUi> _ui;
    };
    //--------------------------------------------------------------------------
}
