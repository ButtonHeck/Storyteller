#pragma once

#include "editor_ui_compositor.h"
#include "editor_ui_impl_opengl_glfw.h"
#include "Storyteller/settings.h"
#include "Storyteller/window.h"
#include "Storyteller/localization_manager.h"
#include "Storyteller/pointers.h"

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

        bool ReadyToClose() const;

        void SaveSettings(Ptr<Settings> settings) const;
        void LoadSettings(Ptr<Settings> settings);

    private:
        void AddDefaultFont();
        void AddIconsFont();

    private:
        Ptr<Window> _window;
        Ptr<EditorUiImplOpenglGLFW> _uiImpl;
        Ptr<EditorUiCompositor> _compositor;
    };
    //--------------------------------------------------------------------------
}
