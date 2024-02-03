#include "window_glfw.h"
#include "log.h"

#include <GLFW/glfw3.h>

namespace Storyteller
{
    struct WindowUserData
    {
        bool updateContinuously = true;
        bool fullscreen = false;
        int width = 1920;
        int height = 1080;
        std::function<void()> refreshCallback = nullptr;
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

        _window = glfwCreateWindow(1920, 1080, "", nullptr, nullptr);

        glfwSwapInterval(1);
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
            glfwSetWindowMonitor(_window, glfwGetPrimaryMonitor(), 0, 0, monitorW, monitorH, vidmode->refreshRate);
        }
        else
        {
            glfwSetWindowMonitor(_window, nullptr,
                (monitorW - userData->width) / 2,
                (monitorH - userData->height) / 2,
                userData->width, userData->height, GLFW_DONT_CARE);
        }
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

    void WindowGlfw::SaveSettings(Ptr<Settings> settings) const
    {
        int width;
        int height;
        glfwGetWindowSize(_window, &width, &height);

        settings->StartSaveGroup("Window");
        settings->SaveUInt("Width", width);
        settings->SaveUInt("Height", height);
        settings->EndSaveGroup();
    }
    //--------------------------------------------------------------------------

    void WindowGlfw::LoadSettings(Ptr<Settings> settings)
    {
        settings->StartLoadGroup("Window");
        const auto width = settings->GetUInt("Width", 1920);
        const auto height = settings->GetUInt("Height", 1080);
        glfwSetWindowSize(_window, width, height);
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
            }
            }
        );

        glfwSetWindowRefreshCallback(_window, [](GLFWwindow* window) {
            const auto userData = GetUserPointer(window);
            if (userData)
            {
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