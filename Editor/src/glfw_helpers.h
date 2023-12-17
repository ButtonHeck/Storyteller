#pragma once
#include <GLFW/glfw3.h>
#include <string>

namespace Storyteller
{
namespace GlfwHelpers
{
    void SetupHints();
    void EndFrame(GLFWwindow* window, bool updateContinuously);
    void ToggleFullscreen(GLFWwindow* window);
    void Shutdown(GLFWwindow* window);

    struct UserData
    {
        bool updateContinuously = true;
        bool fullscreen = false;
        int defaultWidth = 1920;
        int defaultHeight = 1080;
    };
}
}