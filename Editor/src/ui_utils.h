#pragma once

#include <imgui.h>

#include <initializer_list>
#include <utility>

namespace Storyteller
{
    namespace UiUtils
    {
        class DisableGuard
        {
        public:
            explicit DisableGuard(bool condition);
            ~DisableGuard();

        private:
            const bool _condition;
        };
        //--------------------------------------------------------------------------

        class ItemWidthGuard
        {
        public:
            explicit ItemWidthGuard(float width);
            ~ItemWidthGuard();
        };
        //--------------------------------------------------------------------------

        class StyleColorGuard
        {
        public:
            StyleColorGuard(std::initializer_list<std::pair<ImGuiCol_, ImVec4>>&& colors);
            ~StyleColorGuard();

        private:
            std::initializer_list<std::pair<ImGuiCol_, ImVec4>> _colors;
        };
        //--------------------------------------------------------------------------
    }
}
