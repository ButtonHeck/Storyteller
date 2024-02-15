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
        typedef std::function<void()> RefreshCallbackFn;
        typedef std::function<bool()> CloseCallbackFn;
        typedef std::function<void(int key, int scancode, int action, int mods)> KeyCallbackFn;
        typedef std::function<void(unsigned int codepoint)> CharCallbackFn;

    public:
        enum Mode
        {
            FullscreenMode,
            WindowedFullscreenMode,
            WindowedMode
        };

        static Window* CreateWindow();

        virtual ~Window() = default;

        virtual bool Initialize() = 0;

        virtual void SetTitle(const std::string& title) = 0;

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

        virtual void SetRefreshCallback(RefreshCallbackFn refreshCallback) = 0;
        virtual void SetCloseCallback(CloseCallbackFn closeCallback) = 0;
        virtual void SetKeyCallback(KeyCallbackFn keyCallback) = 0;
        virtual void SetCharCallback(CharCallbackFn charCallback) = 0;

        virtual void BeginBlock() = 0;
        virtual void EndBlock() = 0;

        virtual void SaveSettings(Ptr<Settings> settings) const = 0;
        virtual void LoadSettings(Ptr<Settings> settings) = 0;
    };
    //--------------------------------------------------------------------------
}