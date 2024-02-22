#include "editor_ui.h"
#include "icons_font.h"
#include "ui_utils.h"

#include <imgui.h>

namespace Storyteller
{
    EditorUi::EditorUi(Ptr<Window> window, Ptr<LocalizationManager> localizationManager)
        : _window(window)
        , _uiImpl(new EditorUiImplOpenglGLFW(_window))
        , _compositor(new EditorUiCompositor(_window, localizationManager))
    {}
    //--------------------------------------------------------------------------

    bool EditorUi::Initialize()
    {
        IMGUI_CHECKVERSION();
        ImGui::CreateContext();
        auto& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable | ImGuiConfigFlags_ViewportsEnable;

        _uiImpl->Initialize();

        AddDefaultFont();
        AddIconsFont();

        return true;
    }
    //--------------------------------------------------------------------------

    void EditorUi::NewFrame()
    {
        _uiImpl->NewFrame();
    }
    //--------------------------------------------------------------------------

    void EditorUi::Stylize()
    {
        auto& style = ImGui::GetStyle();
        style.FrameBorderSize = 1.0f;
        style.WindowMenuButtonPosition = ImGuiDir_None;
        style.DisabledAlpha = 0.4f;
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

        {
            UiUtils::StyleVarGuard guard({ 
                {ImGuiStyleVar_WindowRounding, 0.0f}, 
                {ImGuiStyleVar_WindowBorderSize, 0.0f}, 
                {ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f)} 
            });
            ImGui::Begin("Dockspace", nullptr, windowFlags);
        }

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
        _uiImpl->Render();
    }
    //--------------------------------------------------------------------------

    void EditorUi::EndFrame()
    {
        ImGui::EndFrame();
    }
    //--------------------------------------------------------------------------

    void EditorUi::Shutdown()
    {
        _uiImpl->Shutdown();
        ImGui::DestroyContext();
    }
    //--------------------------------------------------------------------------

    bool EditorUi::ReadyToClose() const
    {
        return _compositor->ReadyToClose();
    }
    //--------------------------------------------------------------------------

    bool EditorUi::OnWindowCloseEvent(WindowCloseEvent& event)
    {
        if (!ReadyToClose())
        {
            _window->SetShouldClose(false);
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool EditorUi::OnWindowFramebufferRefreshEvent(WindowFramebufferRefreshEvent& event)
    {
        NewFrame();
        Stylize();
        BeginDockspace();
        Compose();
        EndDockspace();
        Render();
        EndFrame();

        return true;
    }
    //--------------------------------------------------------------------------

    bool EditorUi::OnKeyPressEvent(KeyPressEvent& event)
    {
        return _compositor->OnKeyPressEvent(event);
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
        io.Fonts->AddFontFromFileTTF(std::string(STRTLR_EDITOR_FONT_DIR).append("/OpenSans-Regular.ttf").c_str(), fontSize, nullptr, io.Fonts->GetGlyphRangesCyrillic());
    }
    //--------------------------------------------------------------------------

    void EditorUi::AddIconsFont()
    {
        auto& io = ImGui::GetIO();
        const auto fontSize = 18;
        ImFontConfig iconsConfig;
        iconsConfig.MergeMode = true;
        iconsConfig.GlyphMinAdvanceX = fontSize;
        iconsConfig.PixelSnapH = true;
        iconsConfig.GlyphOffset = ImVec2(0.0f, 0.0f);
        static const ImWchar iconsRanges[] = { ICON_MIN_FK, ICON_MAX_FK, 0 };
        io.Fonts->AddFontFromFileTTF(std::string(STRTLR_EDITOR_FONT_DIR).append("/forkawesome-webfont.ttf").c_str(), fontSize, &iconsConfig, iconsRanges);
    }
    //--------------------------------------------------------------------------
}