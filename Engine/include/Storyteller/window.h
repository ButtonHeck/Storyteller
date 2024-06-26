#pragma once

#include "settings.h"
#include "pointers.h"
#include "event.h"

#include <string>
#include <functional>

namespace Storyteller
{
    class Window
    {
    public:
        static Window* Create();

    public:
        using EventCallbackFn = std::function<void(Event&)>;

    public:
        enum Mode
        {
            FullscreenMode,
            WindowedFullscreenMode,
            WindowedMode
        };

        virtual ~Window() = default;

        virtual bool Initialize() = 0;

        virtual void SetTitle(const std::string& title) = 0;
        virtual void SetIcon(const std::string& path) = 0;

        virtual void SetShouldClose(bool close) = 0;
        virtual bool ShouldClose() const = 0;

        virtual void SetScreenMode(Mode mode) = 0;
        virtual Mode GetScreenMode() const = 0;

        virtual void SetVSync(bool vSync) = 0;
        virtual bool IsVSync() const = 0;

        virtual void ProcessEvents() = 0;
        virtual void SwapBuffers() const = 0;
        virtual void MakeContextCurrent() = 0;
        virtual void* GetImplPointer() const = 0;
        virtual void SetEventCallback(const EventCallbackFn& callback) = 0;

        virtual void SaveSettings(Ptr<Settings> settings) const = 0;
        virtual void LoadSettings(Ptr<Settings> settings) = 0;
    };
    //--------------------------------------------------------------------------
}