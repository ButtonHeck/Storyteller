#pragma once

#include "window.h"
#include "localization_manager.h"
#include "editor_ui_compositor.h"
#include "settings.h"

#include <memory>

namespace Storyteller
{
    class EditorUi
    {
    public:
        typedef std::shared_ptr<EditorUi> Ptr;

        EditorUi(Window::Ptr window, LocalizationManager::Ptr localizationManager);

        bool Initialize();
        void NewFrame();
        void Stylize();
        void BeginDockspace();
        void Compose();
        void EndDockspace();
        void Render();
        void EndFrame();
        void Shutdown();

        void SaveSettings(Settings::Ptr settings) const;
        void LoadSettings(Settings::Ptr settings);

    private:
        void AddDefaultFont();

    private:
        Window::Ptr _window;
        EditorUiCompositor::Ptr _compositor;
    };
    //--------------------------------------------------------------------------
}
