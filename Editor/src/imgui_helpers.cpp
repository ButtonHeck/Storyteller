#include "imgui_helpers.h"

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <boost/locale.hpp>
#include <iostream>

namespace Storyteller
{
namespace ImGuiHelpers
{
    void Initialize(GLFWwindow* window)
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;
        ImGui_ImplGlfw_InitForOpenGL(window, true);
        ImGui_ImplOpenGL3_Init("#version 450");

        const auto fontSize = 18;
        const auto fontDirectory = std::string(STORYTELLER_EDITOR_FONT_DIR) + "/OpenSans-Regular.ttf";
        io.Fonts->AddFontFromFileTTF(fontDirectory.c_str(), fontSize, nullptr, io.Fonts->GetGlyphRangesCyrillic());
    }

    void Customize()
    {
        ImGuiStyle& style = ImGui::GetStyle();
        style.Colors[ImGuiCol_FrameBg] = ImVec4(0.45f, 0.14f, 0.4f, 1.0f);
        style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.595f, 0.375f, 0.632f, 1.0f);
        style.Colors[ImGuiCol_WindowBg] = ImVec4(0.064f, 0.092f, 0.104f, 1.0f);
        style.Colors[ImGuiCol_TitleBg] = ImVec4(0.054f, 0.027f, 0.054f, 1.0f);
        style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.285f, 0.598f, 0.749f, 1.0f);
        style.Colors[ImGuiCol_CheckMark] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.0f, 0.597f, 0.963f, 1.0f);
        style.Colors[ImGuiCol_DockingPreview] = ImVec4(0.853f, 0.853f, 0.853f, 0.8f);
    }

    void NewFrame()
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }

    void EndFrame()
    {
        ImGui::EndFrame();
    }

    void PrepareDockspace()
    {
        const ImGuiWindowFlags window_flags =
            ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        const ImGuiViewport* viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);

        // Important: note that we proceed even if Begin() returns false (aka window is collapsed).
        // This is because we want to keep our DockSpace() active. If a DockSpace() is inactive,
        // all active windows docked into it will lose their parent and become undocked.
        // We cannot preserve the docking relationship between an active window and an inactive docking, otherwise
        // any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("DockSpace Demo", nullptr, window_flags);
        ImGui::PopStyleVar();
        ImGui::PopStyleVar(2);

        // Submit the DockSpace
        const ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
            ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
        }
    }
    void UpdateDocking()
    {
        const ImGuiIO& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
        {
            GLFWwindow* backupContext = glfwGetCurrentContext();
            ImGui::UpdatePlatformWindows();
            ImGui::RenderPlatformWindowsDefault();
            glfwMakeContextCurrent(backupContext);
        }
    }

    void Render()
    {
        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void Shutdown()
    {
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
}
}