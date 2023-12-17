#include "game_controller.h"

#include <iostream>

namespace Storyteller
{
    GameController::GameController(const GameDocument::Ptr gameDocument, const LocalizationManager::Ptr localizationManager)
        : _consoleManager(std::make_shared<ConsoleManager>())
        , _gameDocument(gameDocument)
        , _localizationManager(localizationManager)
    {
    }
    //--------------------------------------------------------------------------

    void GameController::Start()
    {
        auto currentObject = _gameDocument->GetEntryPoint();
        if (!currentObject)
        {
            _consoleManager->ClearConsole();
            _consoleManager->PrintCriticalHint();
            _consoleManager->WaitForKeyboardHit();
            return;
        }

        QuestObject* currentQuestObject = nullptr;
        const auto gameName = _gameDocument->GetGameName();
        auto finalReached = false;

        while (!finalReached)
        {
            if (currentObject->GetObjectType() != QuestObject::GetStaticObjectType())
            {
                _consoleManager->PrintCriticalHint();
                _consoleManager->WaitForKeyboardHit();
                return;
            }

            currentQuestObject = dynamic_cast<QuestObject*>(currentObject.get());
            _consoleManager->ClearConsole();
            _consoleManager->PrintMadeByString();
            _consoleManager->PrintSeparator();
            _consoleManager->PrintMessage(gameName);
            _consoleManager->PrintSeparator();
            _consoleManager->PrintMessage(currentQuestObject->GetText());

            const auto questActions = currentQuestObject->GetActions();
            std::vector<std::string> actionTexts(questActions.size());
            for (auto i = 0; i < questActions.size(); i++)
            {
                const auto object = _gameDocument->GetObject(questActions.at(i));
                if (object->GetObjectType() != ActionObject::GetStaticObjectType())
                {
                    _consoleManager->PrintCriticalHint();
                    _consoleManager->WaitForKeyboardHit();
                    return;
                }

                const auto actionObject = dynamic_cast<ActionObject*>(object.get());
                actionTexts.emplace_back(actionObject->GetText());
            }

            _consoleManager->PrintActions(actionTexts);

            int actionIndex = 0;
            while (!actionIndex || (actionIndex >= questActions.size()))
            {
                _consoleManager->PrintInputHint();

                std::string input;
                std::cin >> input;

                try
                {
                    actionIndex = std::stoi(input) - 1;
                    if (actionIndex >= 0 && actionIndex < questActions.size())
                    {
                        currentObject = _gameDocument->GetObject(questActions.at(actionIndex));
                        if (currentObject->GetObjectType() != QuestObject::GetStaticObjectType())
                        {
                            _consoleManager->PrintCriticalHint();
                            _consoleManager->WaitForKeyboardHit();
                            return;
                        }

                        currentQuestObject = dynamic_cast<QuestObject*>(currentObject.get());
                        finalReached = currentQuestObject->IsFinal();
                    }
                    else
                    {
                        _consoleManager->PrintErrorHint();
                    }
                }
                catch (const std::exception& e)
                {
                    _consoleManager->PrintErrorHint();
                }
            }
        }

        _consoleManager->ClearConsole();
        _consoleManager->PrintMadeByString();
        _consoleManager->PrintSeparator();
        _consoleManager->PrintMessage(gameName);
        _consoleManager->PrintSeparator();
        _consoleManager->PrintMessage(currentQuestObject->GetText());
        _consoleManager->PrintEndHint();
        _consoleManager->WaitForKeyboardHit();
    }
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
}