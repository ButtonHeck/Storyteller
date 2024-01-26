#include "window.h"
#include "log.h"
#include "editor_ui.h"

#include <GLFW/glfw3.h>

// TODO: other file
#define STORYTELLER_DOMAIN "Storyteller"
#define EDITOR_DOMAIN "StorytellerEditor"

namespace Storyteller
{
    struct WindowUserData
    {
        bool updateContinuously = true;
        bool fullscreen = false;
        int width = 1920;
        int height = 1080;
        EditorUi* ui = nullptr;
    };
    //--------------------------------------------------------------------------

    Window::Window(LocalizationManager::Ptr localizationManager)
        : _window(nullptr)
        , _localizationManager(localizationManager)
    {}
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
            _localizationManager->Translate(EDITOR_DOMAIN, "Storyteller Editor").c_str(), nullptr, nullptr);

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
                if (userData->ui)
                {
                    userData->ui->Prepare();
                    userData->ui->Compose();
                    userData->ui->Render();
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

    void Window::Shutdown()
    {
        glfwDestroyWindow(_window);
        glfwTerminate();
    }
    //--------------------------------------------------------------------------

    void Window::MakeContextCurrent()
    {
        glfwMakeContextCurrent(_window);
    }
    //--------------------------------------------------------------------------

    void Window::InjectUi(EditorUi* ui)
    {
        auto userData = reinterpret_cast<WindowUserData*>(glfwGetWindowUserPointer(_window));
        if (userData)
        {
            userData->ui = ui;
        }
    }
    //--------------------------------------------------------------------------

    GLFWwindow* Window::GetGLFWWindow() const
    {
        return _window;
    }
    //--------------------------------------------------------------------------
}