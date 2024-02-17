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

        void SetShouldClose(bool close) override;
        bool ShouldClose() const override;

        void SetScreenMode(Mode mode) override;
        Mode GetScreenMode() const override;

        void SetVSync(bool vSync) override;
        bool IsVSync() const override;

        void ProcessEvents() override;
        void SwapBuffers() const override;
        void MakeContextCurrent() override;
        void* GetImplPointer() const override;
        void SetEventCallback(const EventCallbackFn& callback) override;

        void BeginBlock() override;
        void EndBlock() override;

        void SaveSettings(Ptr<Settings> settings) const override;
        void LoadSettings(Ptr<Settings> settings) override;

    private:
        constexpr static auto _DefaultWidth = 1920;
        constexpr static auto _DefaultHeight = 1080;

        struct UserData
        {
            bool updateContinuously = true;
            Mode screenMode = WindowedMode;
            int width = _DefaultWidth;
            int height = _DefaultHeight;
            int windowedWidth = width;
            int windowedHeight = height;
            bool vSync = true;
            bool blocked = false;
            EventCallbackFn eventCallback;
        };

        static UserData* GetUserPointer(GLFWwindow* window);

    private:
        void InitializeHints() const;
        void InitializeCallbacks() const;

    private:
        GLFWwindow* _window;
    };
    //--------------------------------------------------------------------------
}