#pragma once

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
    }
}
