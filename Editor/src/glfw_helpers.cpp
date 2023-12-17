#include "glfw_helpers.h"

namespace Storyteller
{
namespace GlfwHelpers
{
    void SetupHints()
    {
        glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
        glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);
        glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);
        glfwWindowHint(GLFW_FOCUS_ON_SHOW, GLFW_TRUE);
        glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);
        glfwWindowHint(GLFW_MAXIMIZED, GLFW_FALSE);
    }

    void EndFrame(GLFWwindow* window, bool updateContinuously)
    {
        glfwSwapBuffers(window);
        if (updateContinuously)
            glfwPollEvents();
        else
            glfwWaitEvents();
    }

    void ToggleFullscreen(GLFWwindow* window)
    {
        auto userData = reinterpret_cast<UserData*>(glfwGetWindowUserPointer(window));
        if (!userData)
            return;

        userData->fullscreen = !userData->fullscreen;
        const auto vidmode = glfwGetVideoMode(glfwGetPrimaryMonitor());
        const auto monitorW = vidmode->width;
        const auto monitorH = vidmode->height;
        if (userData->fullscreen)
        {
            glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, monitorW, monitorH, vidmode->refreshRate);
        }
        else
        {
            glfwSetWindowMonitor(window, nullptr,
                (monitorW - userData->defaultWidth) / 2,
                (monitorH - userData->defaultHeight) / 2,
                userData->defaultWidth, userData->defaultHeight, GLFW_DONT_CARE);
        }
    }

    void Shutdown(GLFWwindow* window)
    {
        glfwDestroyWindow(window);
        glfwTerminate();
    }
}
}