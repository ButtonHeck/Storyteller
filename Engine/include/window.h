#pragma once

#include "settings.h"
#include "pointers.h"

#include <string>
#include <functional>

namespace Storyteller
{
    class Window
    {
    public:
        static Window* CreateWindow();

        virtual ~Window() = default;

        virtual bool Initialize() = 0;
        virtual void SetTitle(const std::string& title) = 0;
        virtual bool ShouldClose() const = 0;
        virtual void SetShouldClose(bool close) = 0;
        virtual void SetFullscreen(bool fullscreen) = 0;
        virtual void ProcessEvents() = 0;
        virtual void SwapBuffers() const = 0;
        virtual void MakeContextCurrent() = 0;
        virtual void SetRefreshCallback(std::function<void()> refreshCallback) = 0;
        virtual void* GetImplPointer() const = 0;

        virtual void SaveSettings(Ptr<Settings> settings) const = 0;
        virtual void LoadSettings(Ptr<Settings> settings) = 0;
    };
    //--------------------------------------------------------------------------
}