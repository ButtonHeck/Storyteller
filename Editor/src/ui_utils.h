#pragma once

#include <imgui.h>

#include <variant>
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
            explicit StyleColorGuard(std::initializer_list<std::pair<ImGuiCol_, ImVec4>>&& colors);
            ~StyleColorGuard();

        private:
            std::initializer_list<std::pair<ImGuiCol_, ImVec4>> _colors;
        };
        //--------------------------------------------------------------------------

        class StyleVarGuard
        {
        public:
            explicit StyleVarGuard(std::initializer_list<std::pair<ImGuiStyleVar_, std::variant<float, ImVec2>>>&& variables);
            ~StyleVarGuard();

        private:
            std::initializer_list<std::pair<ImGuiStyleVar_, std::variant<float, ImVec2>>> _variables;
        };
        //--------------------------------------------------------------------------

        class IDGuard
        {
        public:
            explicit IDGuard(int id);
            ~IDGuard();

        private:
            const int _id;
        };
        //--------------------------------------------------------------------------

        class GroupGuard
        {
        public:
            GroupGuard();
            ~GroupGuard();
        };
    }
}
