#include "console_manager.h"

#include <iostream>
#include <conio.h>
#ifdef _WIN32
#include <Windows.h>
#endif

namespace Storyteller
{
    ConsoleManager::ConsoleManager(const LocalizationManager::Ptr localizationManager, char separator)
        : _localizationManager(localizationManager)
        , _separator(separator)
    {}
    //--------------------------------------------------------------------------

    void ConsoleManager::SetSeparator(char separator)
    {
        _separator = separator;
    }
    //--------------------------------------------------------------------------

    void ConsoleManager::StartNewFrame(const std::string& gameNameTranslated) const
    {
        ClearConsole();
        PrintSeparator();
        PrintMadeByString();
        PrintSeparator();
        PrintMessage(gameNameTranslated);
        PrintSeparator();
        PrintNewLine();
    }
    //--------------------------------------------------------------------------

    void ConsoleManager::ClearConsole() const
    {
#if defined _WIN32
        system("cls");
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
        std::cout << "\x1B[2J\x1B[H";
#endif
    }
    //--------------------------------------------------------------------------

    void ConsoleManager::PrintMadeByString() const
    {
        std::cout << _localizationManager->Translate("Storyteller", "Made with Storyteller engine") << std::endl;
    }
    //--------------------------------------------------------------------------

    void ConsoleManager::PrintSeparator() const
    {
        int width;

#if defined _WIN32
        CONSOLE_SCREEN_BUFFER_INFO consoleInfo;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &consoleInfo);
        width = consoleInfo.srWindow.Right - consoleInfo.srWindow.Left + 1;
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
        width = 30;
#endif

        for (auto i = 0; i < width; i++)
        {
            std::cout << _separator;
        }

        std::cout << std::endl;
    }
    //--------------------------------------------------------------------------

    void ConsoleManager::PrintMessage(const std::string& message) const
    {
        std::cout << message << std::endl;
    }
    //--------------------------------------------------------------------------

    void ConsoleManager::PrintActions(const std::vector<std::string>& actions) const
    {
        for (auto i = 0; i < actions.size(); i++)
        {
            std::cout << i + 1 << ": " << actions.at(i) << std::endl;
        }
    }
    //--------------------------------------------------------------------------

    void ConsoleManager::PrintInputHint() const
    {
        std::cout << _localizationManager->Translate("Storyteller", "Enter action: ");
    }
    //--------------------------------------------------------------------------

    void ConsoleManager::PrintErrorHint(const std::string& details) const
    {
        std::cout << _localizationManager->Translate("Storyteller", "Error: ") << details << std::endl;
    }
    //--------------------------------------------------------------------------

    void ConsoleManager::PrintCriticalHint(const std::string& details, bool waitForKeyboardHit) const
    {
        std::cout << _localizationManager->Translate("Storyteller", "Critical error: ") << details << std::endl;

        if (waitForKeyboardHit)
        {
            WaitForKeyboardHit();
        }
    }
    //--------------------------------------------------------------------------

    void ConsoleManager::PrintEndHint() const
    {
        std::cout << _localizationManager->Translate("Storyteller", "Game is over!") << std::endl;
    }
    //--------------------------------------------------------------------------

    void ConsoleManager::WaitForKeyboardHit() const
    {
        std::cout << _localizationManager->Translate("Storyteller", "Press any key...") << std::endl;
        while (!_kbhit()) {}
    }
    //--------------------------------------------------------------------------

    void ConsoleManager::PrintNewLine() const
    {
        std::cout << std::endl;
    }
    //--------------------------------------------------------------------------

    std::string ConsoleManager::ReadInput() const
    {
        std::string input;
        std::cin >> input;
        return input;
    }
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
}