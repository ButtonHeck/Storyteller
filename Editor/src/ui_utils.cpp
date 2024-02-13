#include "ui_utils.h"

#include <imgui_internal.h>

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
        //--------------------------------------------------------------------------


        StyleColorGuard::StyleColorGuard(std::initializer_list<std::pair<ImGuiCol_, ImVec4>>&& colors)
            : _colors(colors)
        {
            for (const auto& value : _colors)
            {
                ImGui::PushStyleColor(value.first, value.second);
            }
        }
        //--------------------------------------------------------------------------

        StyleColorGuard::~StyleColorGuard()
        {
            ImGui::PopStyleColor(_colors.size());
        }
        //--------------------------------------------------------------------------
        //--------------------------------------------------------------------------


        StyleVarGuard::StyleVarGuard(std::initializer_list<std::pair<ImGuiStyleVar_, std::variant<float, ImVec2>>>&& variables)
            : _variables(variables)
        {
            for (const auto& value : _variables)
            {
                const auto variableInfo = ImGui::GetStyleVarInfo(value.first);

                if (variableInfo->Count == 1)
                {
                    ImGui::PushStyleVar(value.first, std::get<float>(value.second));
                }
                else if (variableInfo->Count == 2)
                {
                    ImGui::PushStyleVar(value.first, std::get<ImVec2>(value.second));
                }
            }
        }
        //--------------------------------------------------------------------------

        StyleVarGuard::~StyleVarGuard()
        {
            ImGui::PopStyleVar(_variables.size());
        }
        //--------------------------------------------------------------------------
        //--------------------------------------------------------------------------


        IDGuard::IDGuard(int id)
            : _id(id)
        {
            ImGui::PushID(_id);
        }
        //--------------------------------------------------------------------------

        IDGuard::~IDGuard()
        {
            ImGui::PopID();
        }
        //--------------------------------------------------------------------------
        //--------------------------------------------------------------------------


        GroupGuard::GroupGuard()
        {
            ImGui::BeginGroup();
        }
        //--------------------------------------------------------------------------

        GroupGuard::~GroupGuard()
        {
            ImGui::EndGroup();
        }
        //--------------------------------------------------------------------------
        //--------------------------------------------------------------------------


        void SetItemTooltip(const std::string& text)
        {
            if (ImGui::IsItemHovered(ImGuiHoveredFlags_AllowWhenDisabled))
            {
                ImGui::SetTooltip(text.c_str());
            }
        }
        //--------------------------------------------------------------------------
    }
}