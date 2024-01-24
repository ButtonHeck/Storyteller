#pragma once

#include "localization_manager.h"

#include <memory>

struct GLFWwindow;

namespace Storyteller
{
    class EditorUi;

    class Window
    {
    public:
        typedef std::shared_ptr<Window> Ptr;

        Window(LocalizationManager::Ptr localizationManager);

        bool Initialize();
        bool ShouldClose() const;
        void SetShouldClose(bool close);
        void ToggleFullscreen();
        void ProcessEvents();
        void SwapBuffers() const;
        void Shutdown();
        void MakeContextCurrent();
        void InjectUi(EditorUi* ui);
        GLFWwindow* GetGLFWWindow() const;

    private:
        GLFWwindow* _window;
        LocalizationManager::Ptr _localizationManager;
    };
    //--------------------------------------------------------------------------
}
