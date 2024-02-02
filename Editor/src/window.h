#pragma once

#include "localization_manager.h"
#include "settings.h"
#include "pointers.h"

#include <functional>

struct GLFWwindow;

namespace Storyteller
{
    class Window
    {
    public:
        Window(Ptr<LocalizationManager> localizationManager);
        ~Window();

        bool Initialize();
        bool ShouldClose() const;
        void SetShouldClose(bool close);
        void ToggleFullscreen();
        void ProcessEvents();
        void SwapBuffers() const;
        void MakeContextCurrent();
        void SetRefreshCallback(std::function<void()> refreshCallback);
        GLFWwindow* GetGLFWWindow() const;

        void SaveSettings(Ptr<Settings> settings) const;
        void LoadSettings(Ptr<Settings> settings);

    private:
        GLFWwindow* _window;
        Ptr<LocalizationManager> _localizationManager;
    };
    //--------------------------------------------------------------------------
}
