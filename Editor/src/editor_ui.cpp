#include "editor_ui.h"
#include "icons_font.h"
#include "ui_utils.h"

#include <imgui.h>

namespace Storyteller
{
    EditorUi::EditorUi(const Ptr<Window> window, const Ptr<I18N::Manager> i18nManager)
        : _window(window)
        , _uiImpl(EditorUiImpl::CreateImpl(_window))
        , _compositor(CreatePtr<EditorUiCompositor>(_window, i18nManager))
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

    void EditorUi::LoopIteration()
    {
        NewFrame();
        Stylize();
        BeginApplicationArea();
        BeginMainWorkingArea();
        Compose();
        EndMainWorkingArea();
        ComposeStatusBar();
        EndApplicationArea();
        Render();
        EndFrame();
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

        style.Colors[ImGuiCol_FrameBg] = ImColor(122, 66, 41, 138);
        style.Colors[ImGuiCol_FrameBgHovered] = ImColor(250, 157, 66, 171);
        style.Colors[ImGuiCol_FrameBgActive] = ImColor(250, 157, 66, 171);
        style.Colors[ImGuiCol_TitleBgActive] = ImColor(127, 107, 41, 200);
        style.Colors[ImGuiCol_CheckMark] = ImColor(199, 170, 47, 255);
        style.Colors[ImGuiCol_SliderGrab] = ImColor(199, 150, 47, 255);
        style.Colors[ImGuiCol_SliderGrabActive] = ImColor(210, 160, 47, 255);
        style.Colors[ImGuiCol_Button] = ImColor(208, 129, 78, 170);
        style.Colors[ImGuiCol_ButtonHovered] = ImColor(245, 152, 92, 200);
        style.Colors[ImGuiCol_ButtonActive] = ImColor(245, 152, 92, 250);
        style.Colors[ImGuiCol_Header] = ImColor(186, 149, 71, 130);
        style.Colors[ImGuiCol_HeaderHovered] = ImColor(207, 166, 80, 155);
        style.Colors[ImGuiCol_HeaderActive] = ImColor(230, 175, 67, 167);
        style.Colors[ImGuiCol_Separator] = ImColor(128, 110, 110, 128);
        style.Colors[ImGuiCol_SeparatorHovered] = ImColor(162, 41, 41, 128);
        style.Colors[ImGuiCol_SeparatorActive] = ImColor(181, 46, 46, 141);
        style.Colors[ImGuiCol_ResizeGrip] = ImColor(250, 138, 66, 51);
        style.Colors[ImGuiCol_ResizeGripHovered] = ImColor(234, 89, 63, 171);
        style.Colors[ImGuiCol_ResizeGripActive] = ImColor(250, 95, 66, 229);
        style.Colors[ImGuiCol_Tab] = ImColor(140, 80, 49, 220);
        style.Colors[ImGuiCol_TabHovered] = ImColor(182, 90, 43, 220);
        style.Colors[ImGuiCol_TabActive] = ImColor(160, 60, 28, 220);
        style.Colors[ImGuiCol_TabUnfocusedActive] = ImColor(108, 64, 42, 255);
        style.Colors[ImGuiCol_TextSelectedBg] = ImColor(250, 238, 66, 89);
    }
    //--------------------------------------------------------------------------

    void EditorUi::BeginApplicationArea()
    {
        const auto applicationWindowFlags =
            ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        const auto viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowSize(viewport->WorkSize);
        ImGui::SetNextWindowViewport(viewport->ID);

        UiUtils::StyleVarGuard guard({
            {ImGuiStyleVar_WindowRounding, 0.0f},
            {ImGuiStyleVar_WindowBorderSize, 0.0f},
            {ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f)}
        });
        ImGui::Begin("ApplicationWindow", nullptr, applicationWindowFlags);
    }
    //--------------------------------------------------------------------------

    void EditorUi::BeginMainWorkingArea()
    {
        const auto workingAreaFlags =
            ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoTitleBar |
            ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove |
            ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;

        const auto viewport = ImGui::GetMainViewport();
        ImGui::SetNextWindowPos(viewport->WorkPos);
        ImGui::SetNextWindowViewport(viewport->ID);

        {
            UiUtils::StyleVarGuard guard({ 
                {ImGuiStyleVar_WindowRounding, 0.0f}, 
                {ImGuiStyleVar_WindowBorderSize, 0.0f}, 
                {ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f)} 
            });
            ImGui::BeginChild("MainWorkingArea", ImVec2(viewport->WorkSize.x, viewport->WorkSize.y - 40), false, workingAreaFlags);
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

    void EditorUi::EndMainWorkingArea()
    {
        ImGui::EndChild();
    }
    //--------------------------------------------------------------------------

    void EditorUi::ComposeStatusBar()
    {
        UiUtils::StyleVarGuard styleVarGuard({
            {ImGuiStyleVar_WindowRounding, 0.0f},
            {ImGuiStyleVar_WindowBorderSize, 0.0f},
            {ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f)}
        });

        UiUtils::StyleColorGuard styleColorGuard({ { ImGuiCol_ChildBg, ImGui::GetStyleColorVec4(ImGuiCol_MenuBarBg)} });

        if (ImGui::BeginChild("StatusBar", ImGui::GetContentRegionAvail(), false, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoDocking))
        {
            ImGui::EndChild();
        }
    }
    //--------------------------------------------------------------------------

    void EditorUi::EndApplicationArea()
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

    bool EditorUi::OnWindowCloseEvent(WindowCloseEvent& event)
    {
        return _compositor->OnWindowCloseEvent(event);
    }
    //--------------------------------------------------------------------------

    bool EditorUi::OnWindowFramebufferRefreshEvent(WindowFramebufferRefreshEvent& event)
    {
        LoopIteration();

        return true;
    }
    //--------------------------------------------------------------------------

    bool EditorUi::OnKeyPressEvent(KeyPressEvent& event)
    {
        return _compositor->OnKeyPressEvent(event);
    }
    //--------------------------------------------------------------------------

    void EditorUi::SaveSettings(const Ptr<Settings> settings) const
    {
        settings->StartSaveGroup("EditorUi");
        _compositor->SaveSettings(settings);
        settings->EndSaveGroup();
    }
    //--------------------------------------------------------------------------

    void EditorUi::LoadSettings(const Ptr<Settings> settings)
    {
        settings->StartLoadGroup("EditorUi");
        _compositor->LoadSettings(settings);
        settings->EndLoadGroup();
    }
    //--------------------------------------------------------------------------

    void EditorUi::Shutdown()
    {
        _uiImpl->Shutdown();
        ImGui::DestroyContext();
    }
    //--------------------------------------------------------------------------

    void EditorUi::AddDefaultFont()
    {
        // TODO: extract to separate font class
        auto& io = ImGui::GetIO();
        const auto fontSize = 18;
        io.Fonts->AddFontFromFileTTF(Utils::Concatenate(STRTLR_EDITOR_FONT_DIR, "/OpenSans-Regular.ttf").c_str(), fontSize, nullptr, io.Fonts->GetGlyphRangesCyrillic());
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
        io.Fonts->AddFontFromFileTTF(Utils::Concatenate(STRTLR_EDITOR_FONT_DIR, "/forkawesome-webfont.ttf").c_str(), fontSize, &iconsConfig, iconsRanges);
    }
    //--------------------------------------------------------------------------
}