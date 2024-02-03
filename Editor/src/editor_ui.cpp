#include "editor_ui.h"

#include <GLFW/glfw3.h>
#include <imgui.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace Storyteller
{
    EditorUi::EditorUi(Ptr<Window> window, Ptr<LocalizationManager> localizationManager)
        : _window(window)
        , _compositor(new EditorUiCompositor(_window, localizationManager))
    {}
    //--------------------------------------------------------------------------

    bool EditorUi::Initialize()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        auto& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;

        //TODO: extract to separate class (eg EditorUiGlfwOpengl)
        ImGui_ImplGlfw_InitForOpenGL(reinterpret_cast<GLFWwindow*>(_window->GetImplPointer()), true);
        ImGui_ImplOpenGL3_Init("#version 450");

        AddDefaultFont();

        return true;
    }
    //--------------------------------------------------------------------------

    void EditorUi::NewFrame()
    {
        //TODO: extract to impl ui
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();
    }
    //--------------------------------------------------------------------------

    void EditorUi::Stylize()
    {
        auto& style = ImGui::GetStyle();
        style.FrameBorderSize = 1.0f;
        style.WindowMenuButtonPosition = ImGuiDir_None;
        //style.Colors[ImGuiCol_FrameBg] = ImVec4(0.45f, 0.14f, 0.4f, 1.0f);
        //style.Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.595f, 0.375f, 0.632f, 1.0f);
        //style.Colors[ImGuiCol_WindowBg] = ImVec4(0.064f, 0.092f, 0.104f, 1.0f);
        //style.Colors[ImGuiCol_TitleBg] = ImVec4(0.054f, 0.027f, 0.054f, 1.0f);
        //style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.285f, 0.598f, 0.749f, 1.0f);
        //style.Colors[ImGuiCol_CheckMark] = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
        //style.Colors[ImGuiCol_SliderGrab] = ImVec4(1.0f, 0.597f, 0.963f, 1.0f);
        //style.Colors[ImGuiCol_DockingPreview] = ImVec4(0.853f, 0.853f, 0.853f, 0.8f);
    }
    //--------------------------------------------------------------------------

    void EditorUi::BeginDockspace()
    {
        const auto windowFlags =
            ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        const auto viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
        ImGui::Begin("Dockspace", nullptr, windowFlags);
        ImGui::PopStyleVar(3);

        const auto& io = ImGui::GetIO();
        if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
        {
            const auto dockspaceId = ImGui::GetID("EditorDockspace");
            ImGui::DockSpace(dockspaceId, ImVec2(0.0f, 0.0f), ImGuiDockNodeFlags_None);
        }
    }
    //--------------------------------------------------------------------------

    void EditorUi::Compose()
    {
        _compositor->Compose();
    }
    //--------------------------------------------------------------------------

    void EditorUi::EndDockspace()
    {
        ImGui::End();
    }
    //--------------------------------------------------------------------------

    void EditorUi::Render()
    {
        ImGui::Render();

        //TODO: extract to impl class
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

    void EditorUi::EndFrame()
    {
        ImGui::EndFrame();
    }
    //--------------------------------------------------------------------------

    void EditorUi::Shutdown()
    {
        //TODO: extract to impl class
        ImGui_ImplOpenGL3_Shutdown();
        ImGui_ImplGlfw_Shutdown();
        ImGui::DestroyContext();
    }
    //--------------------------------------------------------------------------

    void EditorUi::SaveSettings(Ptr<Settings> settings) const
    {
        settings->StartSaveGroup("EditorUi");
        _compositor->SaveSettings(settings);
        settings->EndSaveGroup();
    }
    //--------------------------------------------------------------------------

    void EditorUi::LoadSettings(Ptr<Settings> settings)
    {
        settings->StartLoadGroup("EditorUi");
        _compositor->LoadSettings(settings);
        settings->EndLoadGroup();
    }
    //--------------------------------------------------------------------------

    void EditorUi::AddDefaultFont()
    {
        // TODO: extract to separate font class
        auto& io = ImGui::GetIO();
        const auto fontSize = 18;
        const auto fontDirectory = std::string(STRTLR_EDITOR_FONT_DIR) + "/OpenSans-Regular.ttf";
        io.Fonts->AddFontFromFileTTF(fontDirectory.c_str(), fontSize, nullptr, io.Fonts->GetGlyphRangesCyrillic());
    }
    //--------------------------------------------------------------------------
}