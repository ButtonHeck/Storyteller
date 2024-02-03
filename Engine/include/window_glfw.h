#pragma once

#include "window.h"
#include "settings.h"
#include "pointers.h"

#include <functional>

struct GLFWwindow;

namespace Storyteller
{
    class WindowGlfw : public Window
    {
    public:
        WindowGlfw();
        ~WindowGlfw();

        bool Initialize() override;
        void SetTitle(const std::string& title) override;
        bool ShouldClose() const override;
        void SetShouldClose(bool close) override;
        void SetFullscreen(bool fullscreen) override;
        void ProcessEvents() override;
        void SwapBuffers() const override;
        void MakeContextCurrent() override;
        void SetRefreshCallback(std::function<void()> refreshCallback) override;
        void* GetImplPointer() const override;

        void SaveSettings(Ptr<Settings> settings) const override;
        void LoadSettings(Ptr<Settings> settings) override;

    private:
        void InitializeHints() const;
        void InitializeCallbacks() const;

    private:
        GLFWwindow* _window;
    };
    //--------------------------------------------------------------------------
}