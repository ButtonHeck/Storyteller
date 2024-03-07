#pragma once

#include "editor_ui_compositor.h"
#include "editor_ui_impl.h"
#include "Storyteller/settings.h"
#include "Storyteller/window.h"
#include "Storyteller/localization_manager.h"
#include "Storyteller/pointers.h"
#include "Storyteller/window_event.h"
#include "Storyteller/key_event.h"

namespace Storyteller
{
    class EditorUi
    {
    public:
        EditorUi(Ptr<Window> window, Ptr<LocalizationManager> localizationManager);

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

        void SaveSettings(Ptr<Settings> settings) const;
        void LoadSettings(Ptr<Settings> settings);

        void Shutdown();

    private:
        void AddDefaultFont();
        void AddIconsFont();

    private:
        Ptr<Window> _window;
        Ptr<EditorUiImpl> _uiImpl;
        Ptr<EditorUiCompositor> _compositor;
    };
    //--------------------------------------------------------------------------
}
