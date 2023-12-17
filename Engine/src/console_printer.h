#pragma once

#include <string>
#include <memory>
#include <vector>

namespace Storyteller
{
    class ConsoleManager
    {
    public:
        typedef std::shared_ptr<ConsoleManager> Ptr;

        explicit ConsoleManager(char separator = '*');

        void SetSeparator(char separator);

        void ClearConsole() const;
        void PrintMadeByString() const;
        void PrintSeparator() const;
        void PrintMessage(const std::string& message) const;
        void PrintActions(const std::vector<std::string>& actions) const;
        void PrintInputHint() const;
        void PrintErrorHint() const;
        void PrintCriticalHint() const;
        void PrintEndHint() const;
        void WaitForKeyboardHit() const;

    private:
        char _separator;
    };
    //--------------------------------------------------------------------------
}
