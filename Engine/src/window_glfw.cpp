#include "window_glfw.h"
#include "log.h"
#include "window_event.h"
#include "key_event.h"

#include <GLFW/glfw3.h>

namespace Storyteller
{
    constexpr auto defaultWidth = 1920;
    constexpr auto defaultHeight = 1080;

    struct WindowUserData
    {
        bool updateContinuously = true;
        Window::Mode screenMode = Window::WindowedMode;
        int width = defaultWidth;
        int height = defaultHeight;
        int windowedWidth = width;
        int windowedHeight = height;
        bool vSync = true;
        bool blocked = false;
        Window::EventCallbackFn eventCallback;
    };
    //--------------------------------------------------------------------------

    WindowUserData* GetUserPointer(GLFWwindow* window)
    {
        return reinterpret_cast<WindowUserData*>(glfwGetWindowUserPointer(window));
    }
    //--------------------------------------------------------------------------

    WindowGlfw::WindowGlfw()
        : _window(nullptr)
    {}
    //--------------------------------------------------------------------------

    WindowGlfw::~WindowGlfw()
    {
        glfwDestroyWindow(_window);
        glfwTerminate();
    }
    //--------------------------------------------------------------------------

    bool WindowGlfw::Initialize()
    {
        if (!glfwInit())
        {
            return false;
        }

        InitializeHints();

        _window = glfwCreateWindow(defaultWidth, defaultHeight, "", nullptr, nullptr);

        glfwSetWindowUserPointer(_window, new WindowUserData());
        MakeContextCurrent();

        InitializeCallbacks();

        return true;
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::SetTitle(const std::string& title)
    {
        glfwSetWindowTitle(_window, title.c_str());
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::SetShouldClose(bool close)
    {
        glfwSetWindowShouldClose(_window, close);
    }
    //--------------------------------------------------------------------------

    bool WindowGlfw::ShouldClose() const
    {
        return glfwWindowShouldClose(_window);
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::SetScreenMode(Mode mode)
    {
        const auto userData = GetUserPointer(_window);
        if (!userData || userData->screenMode == mode)
        {
            return;
        }

        userData->screenMode = mode;

        const auto monitor = glfwGetPrimaryMonitor();
        const auto videoMode = glfwGetVideoMode(monitor);
        const auto monitorWidth = videoMode->width;
        const auto monitorHeight = videoMode->height;

        if (mode == FullscreenMode)
        {
            glfwSetWindowMonitor(_window, monitor, 0, 0, monitorWidth, monitorHeight, videoMode->refreshRate);
        }
        else if (mode == WindowedFullscreenMode)
        {
            int workingAreaX;
            int workingAreaY;
            int workingAreaWidth;
            int workingAreaHeight;
            glfwGetMonitorWorkarea(monitor, &workingAreaX, &workingAreaY, &workingAreaWidth, &workingAreaHeight);
            glfwSetWindowMonitor(_window, nullptr, workingAreaX, workingAreaY, workingAreaWidth, workingAreaHeight, videoMode->refreshRate);
        }
        else
        {
            glfwSetWindowMonitor(_window, nullptr,
                (monitorWidth - userData->windowedWidth) / 2,
                (monitorHeight - userData->windowedHeight) / 2,
                userData->windowedWidth, userData->windowedHeight, videoMode->refreshRate);
        }
    }
    //--------------------------------------------------------------------------

    Window::Mode WindowGlfw::GetScreenMode() const
    {
        const auto userData = GetUserPointer(_window);
        if (userData)
        {
            return userData->screenMode;
        }

        return WindowedMode;
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::SetVSync(bool vSync)
    {
        const auto userData = GetUserPointer(_window);
        if (userData)
        {
            userData->vSync = vSync;
        }

        glfwSwapInterval(vSync ? 1 : 0);
    }
    //--------------------------------------------------------------------------

    bool WindowGlfw::IsVSync() const
    {
        const auto userData = GetUserPointer(_window);
        if (userData)
        {
            return userData->vSync;
        }

        return true;
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::ProcessEvents()
    {
        const auto userData = GetUserPointer(_window);
        if (userData->updateContinuously)
        {
            glfwPollEvents();
        }
        else
        {
            glfwWaitEvents();
        }
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::SwapBuffers() const
    {
        glfwSwapBuffers(_window);
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::MakeContextCurrent()
    {
        glfwMakeContextCurrent(_window);
    }
    //--------------------------------------------------------------------------

    void* WindowGlfw::GetImplPointer() const
    {
        return _window;
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::SetEventCallback(const EventCallbackFn& callback)
    {
        const auto userData = GetUserPointer(_window);
        if (userData)
        {
            userData->eventCallback = callback;
        }
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::BeginBlock()
    {
        const auto userData = GetUserPointer(_window);
        if (userData)
        {
            userData->blocked = true;
        }
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::EndBlock()
    {
        const auto userData = GetUserPointer(_window);
        if (userData)
        {
            userData->blocked = false;
        }
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::SaveSettings(Ptr<Settings> settings) const
    {
        int width;
        int height;
        glfwGetWindowSize(_window, &width, &height);

        const auto userData = GetUserPointer(_window);

        settings->StartSaveGroup("Window");
        settings->SaveUInt("Width", width);
        settings->SaveUInt("Height", height);
        settings->SaveUInt("WindowedWidth", userData ? userData->windowedWidth : defaultWidth);
        settings->SaveUInt("WindowedHeight", userData ? userData->windowedHeight : defaultHeight);
        settings->SaveInt("ScreenMode", GetScreenMode());
        settings->SaveBool("VSync", IsVSync());
        settings->SaveBool("UpdateContinuously", userData ? userData->updateContinuously : true);
        settings->EndSaveGroup();
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::LoadSettings(Ptr<Settings> settings)
    {
        settings->StartLoadGroup("Window");
        const auto width = settings->GetUInt("Width", defaultWidth);
        const auto height = settings->GetUInt("Height", defaultHeight);
        const auto windowedWidth = settings->GetUInt("WindowedWidth", defaultWidth);
        const auto windowedHeight = settings->GetUInt("WindowedHeight", defaultHeight);
        const auto screenMode = Window::Mode(settings->GetInt("ScreenMode", WindowedMode));
        const auto vSync = settings->GetBool("VSync", true);
        const auto updateContinuously = settings->GetBool("UpdateContinuously", true);

        const auto userData = GetUserPointer(_window);
        if (userData)
        {
            userData->windowedWidth = windowedWidth;
            userData->windowedHeight = windowedHeight;
            userData->updateContinuously = updateContinuously;
        }

        SetVSync(vSync);
        SetScreenMode(screenMode);

        if (screenMode == WindowedMode)
        {
            glfwSetWindowSize(_window, width, height);
        }

        settings->EndLoadGroup();
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::InitializeHints() const
    {
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
        glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
        glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);
        glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
        glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::InitializeCallbacks() const
    {
        glfwSetFramebufferSizeCallback(_window, [](GLFWwindow* window, int width, int height) {
            const auto userData = GetUserPointer(window);
            if (userData)
            {
                userData->width = width;
                userData->height = height;

                if (userData->screenMode == WindowedMode)
                {
                    userData->windowedWidth = width;
                    userData->windowedHeight = height;
                }

                if (userData->eventCallback)
                {
                    WindowFramebufferResizeEvent event(width, height);
                    userData->eventCallback(event);
                }
            }
            }
        );

        glfwSetWindowRefreshCallback(_window, [](GLFWwindow* window) {
            const auto userData = GetUserPointer(window);
            if (userData)
            {
                if (userData->blocked)
                {
                    return;
                }

                if (userData->eventCallback)
                {
                    WindowFramebufferRefreshEvent event;
                    userData->eventCallback(event);
                }
            }

            glfwSwapBuffers(window);
            }
        );

        glfwSetWindowCloseCallback(_window, [](GLFWwindow* window) {
            const auto userData = GetUserPointer(window);
            if (userData)
            {
                if (userData->eventCallback)
                {
                    WindowCloseEvent event;
                    userData->eventCallback(event);
                }
            }
            }
        );

        glfwSetKeyCallback(_window, [](GLFWwindow* window, int key, int scancode, int action, int mods) {
            const auto userData = GetUserPointer(window);
            if (userData)
            {
                if (userData->eventCallback)
                {
                    //todo
                }
            }
            }
        );

        glfwSetCharCallback(_window, [](GLFWwindow* window, unsigned int codepoint) {
            const auto userData = GetUserPointer(window);
            if (userData)
            {
                if (userData->eventCallback)
                {
                    KeyCharEvent event(codepoint);
                    userData->eventCallback(event);
                }
            }
            }
        );
    }
    //--------------------------------------------------------------------------
}