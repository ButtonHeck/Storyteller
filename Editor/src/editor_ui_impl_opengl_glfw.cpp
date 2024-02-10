#include "editor_ui_impl_opengl_glfw.h"

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace Storyteller
{
    EditorUiImplOpenglGLFW::EditorUiImplOpenglGLFW(Ptr<Window> window)
        : _window(window)
    {}
    //--------------------------------------------------------------------------

    void EditorUiImplOpenglGLFW::Initialize()
    {
        ImGui_ImplGlfw_InitForOpenGL(reinterpret_cast<GLFWwindow*>(_window->GetImplPointer()), true);
        ImGui_ImplOpenGL3_Init("#version 450");
    }
    //--------------------------------------------------------------------------

    void EditorUiImplOpenglGLFW::NewFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }
    //--------------------------------------------------------------------------

    void EditorUiImplOpenglGLFW::Render()
    {
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        const auto& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            const auto backupContext = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backupContext);
        }
    }
    //--------------------------------------------------------------------------

    void EditorUiImplOpenglGLFW::Shutdown()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
    }
    //--------------------------------------------------------------------------
}