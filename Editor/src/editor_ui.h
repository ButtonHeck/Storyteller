#pragma once

#include "window.h"
#include "localization_manager.h"
#include "editor_ui_compositor.h"
#include "settings.h"
#include "pointers.h"

namespace Storyteller
{
    class EditorUi
    {
    public:
        EditorUi(Ptr<Window> window, Ptr<LocalizationManager> localizationManager);

        bool Initialize();
        void NewFrame();
        void Stylize();
        void BeginDockspace();
        void Compose();
        void EndDockspace();
        void Render();
        void EndFrame();
        void Shutdown();

        void SaveSettings(Ptr<Settings> settings) const;
        void LoadSettings(Ptr<Settings> settings);

    private:
        void AddDefaultFont();

    private:
        Ptr<Window> _window;
        Ptr<EditorUiCompositor> _compositor;
    };
    //--------------------------------------------------------------------------
}
