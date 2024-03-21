#pragma once

#include "Storyteller/pointers.h"
#include "Storyteller/localization_manager.h"

#include <string>
#include <vector>

namespace Storyteller
{
    class ConsoleManager
    {
    public:
        explicit ConsoleManager(const Ptr<LocalizationManager> localizationManager, char separator = '*');

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
        Ptr<LocalizationManager> const _localizationManager;
        char _separator;
    };
    //--------------------------------------------------------------------------
}
