#include "ui_utils.h"

#include <imgui.h>

namespace Storyteller
{
    namespace UiUtils
    {
        DisableGuard::DisableGuard(bool condition)
            : _condition(condition)
        {
            if (_condition)
            {
                ImGui::BeginDisabled();
            }
        }
        //--------------------------------------------------------------------------

        DisableGuard::~DisableGuard()
        {
            if (_condition)
            {
                ImGui::EndDisabled();
            }
        }
        //--------------------------------------------------------------------------

        ItemWidthGuard::ItemWidthGuard(float width)
        {
            ImGui::PushItemWidth(width);
        }
        //--------------------------------------------------------------------------

        ItemWidthGuard::~ItemWidthGuard()
        {
            ImGui::PopItemWidth();
        }
        //--------------------------------------------------------------------------
    }
}