#include "window.h"
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

    Window::Window(Ptr<LocalizationManager> localizationManager)
        : _window(nullptr)
        , _localizationManager(localizationManager)
    {}
    //--------------------------------------------------------------------------

    Window::~Window()
    {
        glfwDestroyWindow(_window);
        glfwTerminate();
    }
    //--------------------------------------------------------------------------

    bool Window::Initialize()
    {
        if (!glfwInit())
        {
            return false;
        }

        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
        glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
        glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);
        glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
        glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);

        _window = glfwCreateWindow(1920, 1080,
            _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Storyteller Editor").c_str(), nullptr, nullptr);

        glfwSwapInterval(1);
        glfwSetWindowUserPointer(_window, new WindowUserData());
        MakeContextCurrent();

        glfwSetFramebufferSizeCallback(_window, [](GLFWwindow* window, int width, int height) {
            auto userData = reinterpret_cast<WindowUserData*>(glfwGetWindowUserPointer(window));
            if (userData)
            {
                userData->width = width;
                userData->height = height;
            }
        });

        glfwSetWindowRefreshCallback(_window, [](GLFWwindow* window) {
            auto userData = reinterpret_cast<WindowUserData*>(glfwGetWindowUserPointer(window));
            if (userData)
            {
                if (userData->refreshCallback)
                {
                    userData->refreshCallback();
                }
            }

            glfwSwapBuffers(window);
        });

        return true;
    }
    //--------------------------------------------------------------------------

    bool Window::ShouldClose() const
    {
        return glfwWindowShouldClose(_window);
    }
    //--------------------------------------------------------------------------

    void Window::SetShouldClose(bool close)
    {
        glfwSetWindowShouldClose(_window, close);
    }
    //--------------------------------------------------------------------------

    void Window::ToggleFullscreen()
    {
        auto userData = reinterpret_cast<WindowUserData*>(glfwGetWindowUserPointer(_window));
        if (!userData)
        {
            return;
        }

        userData->fullscreen = !userData->fullscreen;
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

    void Window::ProcessEvents()
    {
        const auto userData = reinterpret_cast<WindowUserData*>(glfwGetWindowUserPointer(_window));
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

    void Window::SwapBuffers() const
    {
        glfwSwapBuffers(_window);
    }
    //--------------------------------------------------------------------------

    void Window::MakeContextCurrent()
    {
        glfwMakeContextCurrent(_window);
    }
    //--------------------------------------------------------------------------

    void Window::SetRefreshCallback(std::function<void()> refreshCallback)
    {
        auto userData = reinterpret_cast<WindowUserData*>(glfwGetWindowUserPointer(_window));
        if (userData)
        {
            userData->refreshCallback = refreshCallback;
        }
    }
    //--------------------------------------------------------------------------

    GLFWwindow* Window::GetGLFWWindow() const
    {
        return _window;
    }
    //--------------------------------------------------------------------------

    void Window::SaveSettings(Ptr<Settings> settings) const
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

    void Window::LoadSettings(Ptr<Settings> settings)
    {
        settings->StartLoadGroup("Window");
        const auto width = settings->GetUInt("Width", 1920);
        const auto height = settings->GetUInt("Height", 1080);
        glfwSetWindowSize(_window, width, height);
        settings->EndLoadGroup();
    }
    //--------------------------------------------------------------------------
}