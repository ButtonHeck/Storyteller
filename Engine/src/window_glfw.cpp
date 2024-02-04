#include "window_glfw.h"
#include "log.h"

#include <GLFW/glfw3.h>

namespace Storyteller
{
    constexpr auto defaultWidth = 1920;
    constexpr auto defaultHeight = 1080;

    struct WindowUserData
    {
        bool updateContinuously = true;
        bool fullscreen = false;
        int width = defaultWidth;
        int height = defaultHeight;
        int windowedWidth = width;
        int windowedHeight = height;
        std::function<void()> refreshCallback = nullptr;
        bool vSync = true;
        bool blocked = false;
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

        SetVSync(true);
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

    bool WindowGlfw::ShouldClose() const
    {
        return glfwWindowShouldClose(_window);
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::SetShouldClose(bool close)
    {
        glfwSetWindowShouldClose(_window, close);
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::SetFullscreen(bool fullscreen)
    {
        const auto userData = GetUserPointer(_window);
        if (!userData || userData->fullscreen == fullscreen)
        {
            return;
        }

        userData->fullscreen = fullscreen;
        const auto vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        const auto monitorW = vidmode->width;
        const auto monitorH = vidmode->height;
        if (userData->fullscreen)
        {
            int x, y, w, h;
            glfwGetMonitorWorkarea(glfwGetPrimaryMonitor(), &x, &y, &w, &h);
            glfwSetWindowMonitor(_window, nullptr, x, y, w, h, vidmode->refreshRate);
        }
        else
        {
            glfwSetWindowMonitor(_window, nullptr,
                (monitorW - userData->windowedWidth) / 2,
                (monitorH - userData->windowedHeight) / 2,
                userData->windowedWidth, userData->windowedHeight, GLFW_DONT_CARE);
        }
    }
    //--------------------------------------------------------------------------

    bool WindowGlfw::IsFullscreen() const
    {
        const auto userData = GetUserPointer(_window);
        if (userData)
        {
            return userData->fullscreen;
        }

        return false;
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

    void WindowGlfw::SetRefreshCallback(std::function<void()> refreshCallback)
    {
        const auto userData = GetUserPointer(_window);
        if (userData)
        {
            userData->refreshCallback = refreshCallback;
        }
    }
    //--------------------------------------------------------------------------

    void* WindowGlfw::GetImplPointer() const
    {
        return _window;
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
        settings->SaveBool("Fullscreen", IsFullscreen());
        settings->SaveBool("VSync", IsVSync());
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
        const auto fullscreen = settings->GetBool("Fullscreen", false);
        const auto vSync = settings->GetBool("VSync", true);

        const auto userData = GetUserPointer(_window);
        if (userData)
        {
            userData->windowedWidth = windowedWidth;
            userData->windowedHeight = windowedHeight;
        }

        SetVSync(vSync);
        SetFullscreen(fullscreen);

        if (!fullscreen)
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

                if (!userData->fullscreen)
                {
                    userData->windowedWidth = width;
                    userData->windowedHeight = height;
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

                if (userData->refreshCallback)
                {
                    userData->refreshCallback();
                }
            }

            glfwSwapBuffers(window);
            }
        );
    }
    //--------------------------------------------------------------------------
}