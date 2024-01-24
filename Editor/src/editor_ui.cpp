#include "editor_ui.h"
#include "imgui_helpers.h"

namespace Storyteller
{
    EditorUi::EditorUi(Window::Ptr window)
        : _window(window)
    {}
    //--------------------------------------------------------------------------

    bool EditorUi::Initialize()
    {
        return ImGuiHelpers::Initialize(_window->GetGLFWWindow());
    }
    //--------------------------------------------------------------------------

    void EditorUi::Prepare()
    {
        ImGuiHelpers::NewFrame();
        ImGuiHelpers::PrepareDockspace();
        ImGuiHelpers::Customize();
    }
    //--------------------------------------------------------------------------

    void EditorUi::Compose()
    {
    }
    //--------------------------------------------------------------------------

    void EditorUi::Render()
    {
        ImGuiHelpers::Render();
        ImGuiHelpers::UpdateDocking();
        ImGuiHelpers::EndFrame();
    }
    //--------------------------------------------------------------------------
}