#pragma once

#include "Storyteller/pointers.h"
#include "Storyteller/i18n_manager.h"

#include <string>
#include <vector>

namespace Storyteller
{
    class ConsoleManager
    {
    public:
        explicit ConsoleManager(const Ptr<I18N::Manager> i18nManager, char separator = '*');

        void SetSeparator(char separator);

        void StartNewFrame(const std::string& gameNameTranslated) const;
        void ClearConsole() const;
        void PrintMadeByString() const;
        void PrintSeparator() const;
        void PrintMessage(const std::string& message) const;
        void PrintActions(const std::vector<std::string>& actions) const;
        void PrintInputHint() const;
        void PrintErrorHint(const std::string& details) const;
        void PrintCriticalHint(const std::string& details, bool waitForKeyboardHit = true) const;
        void PrintEndHint() const;
        void WaitForKeyboardHit() const;
        void PrintNewLine() const;
        std::string ReadInput() const;

    private:
        void FillDictionary() const;

    private:
        const Ptr<I18N::Manager> _i18nManager;
        char _separator;
    };
    //--------------------------------------------------------------------------
}
