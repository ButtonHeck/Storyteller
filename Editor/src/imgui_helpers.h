#pragma once

struct GLFWwindow;

namespace Storyteller
{
namespace ImGuiHelpers
{
    bool Initialize(GLFWwindow* window);
    void Customize();

    void NewFrame();
    void EndFrame();

    void PrepareDockspace();
    void UpdateDocking();

    void Render();

    void Shutdown();
}
}