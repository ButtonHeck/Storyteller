#include "window.h"
#include "log.h"

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

        std::shared_ptr<WindowUserData> userData = std::make_shared<WindowUserData>();

        _window = glfwCreateWindow(userData->width, userData->height,
            _localizationManager->Translate(EDITOR_DOMAIN, "Storyteller Editor").c_str(), nullptr, nullptr);

        glfwSwapInterval(1);
        glfwSetWindowUserPointer(_window, userData.get());
        MakeContextCurrent();

        glfwSetFramebufferSizeCallback(_window, [](GLFWwindow* window, int width, int height) {
            auto userData = reinterpret_cast<WindowUserData*>(glfwGetWindowUserPointer(window));
            if (userData)
            {
                userData->width = width;
                userData->height = height;
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

    GLFWwindow* Window::GetGLFWWindow() const
    {
        return _window;
    }
    //--------------------------------------------------------------------------
}