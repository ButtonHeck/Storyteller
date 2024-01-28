#pragma once

namespace Storyteller
{
    namespace UiUtils
    {
        class DisableGuard
        {
        public:
            DisableGuard(bool condition);
            ~DisableGuard();

        private:
            const bool _condition;
        };
        //--------------------------------------------------------------------------
    }
}
