#pragma once

#include "localization_manager.h"

#include <string>
#include <memory>
#include <vector>

namespace Storyteller
{
    class ConsoleManager
    {
    public:
        typedef std::shared_ptr<ConsoleManager> Ptr;

        explicit ConsoleManager(const LocalizationManager::Ptr localizationManager, char separator = '*');

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
        LocalizationManager::Ptr const _localizationManager;
        char _separator;
    };
    //--------------------------------------------------------------------------
}
