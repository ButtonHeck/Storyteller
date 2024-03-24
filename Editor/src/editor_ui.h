#pragma once

#include "editor_ui_compositor.h"
#include "editor_ui_impl.h"
#include "Storyteller/settings.h"
#include "Storyteller/window.h"
#include "Storyteller/i18n_manager.h"
#include "Storyteller/pointers.h"
#include "Storyteller/window_event.h"
#include "Storyteller/key_event.h"

namespace Storyteller
{
    class EditorUi
    {
    public:
        EditorUi(const Ptr<Window> window, const Ptr<I18N::Manager> i18nManager);

        bool Initialize();

        void LoopIteration();

        void NewFrame();
        void Stylize();
        void BeginApplicationArea();
        void BeginMainWorkingArea();
        void Compose();
        void EndMainWorkingArea();
        void ComposeStatusBar();
        void EndApplicationArea();
        void Render();
        void EndFrame();

        bool OnWindowCloseEvent(WindowCloseEvent& event);
        bool OnWindowFramebufferRefreshEvent(WindowFramebufferRefreshEvent& event);
        bool OnKeyPressEvent(KeyPressEvent& event);

        void SaveSettings(const Ptr<Settings> settings) const;
        void LoadSettings(const Ptr<Settings> settings);

        void Shutdown();

    private:
        void AddDefaultFont();
        void AddIconsFont();

    private:
        const Ptr<Window> _window;
        const Ptr<EditorUiImpl> _uiImpl;
        const Ptr<EditorUiCompositor> _compositor;
    };
    //--------------------------------------------------------------------------
}
