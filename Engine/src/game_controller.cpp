#include "game_controller.h"

namespace Storyteller
{
    GameController::GameController(const GameDocument::Ptr gameDocument, const LocalizationManager::Ptr localizationManager)
        : _consoleManager(std::make_shared<ConsoleManager>(localizationManager))
        , _gameDocument(gameDocument)
        , _localizationManager(localizationManager)
        , _gameName(_gameDocument->GetGameName())
        , _gameNameTranslated(_localizationManager->Translate(_gameName, _gameName))
    {}
    //--------------------------------------------------------------------------

    void GameController::Launch()
    {
        const QuestObject* currentQuestObject = nullptr;

        const auto noErrors = MainLoop(currentQuestObject);
        if (noErrors)
        {
            End(currentQuestObject);
        }
    }
    //--------------------------------------------------------------------------

    bool GameController::MainLoop(const QuestObject* currentQuestObject)
    {
        auto currentObject = _gameDocument->GetEntryPoint();
        auto finalReached = false;

        while (!finalReached)
        {
            if (!CheckObject(currentObject, ObjectType::QuestObjectType))
            {
                return false;
            }

            currentQuestObject = dynamic_cast<QuestObject*>(currentObject.get());
            NewFrame(currentQuestObject);

            if (!ProcessActions(currentObject, currentQuestObject, finalReached))
            {
                return false;
            }
        }

        return true;
    }
    //--------------------------------------------------------------------------

    void GameController::End(const QuestObject* currentQuestObject)
    {
        NewFrame(currentQuestObject);
        
        _consoleManager->PrintEndHint();
        _consoleManager->WaitForKeyboardHit();
    }
    //--------------------------------------------------------------------------

    bool GameController::CheckObject(const BasicObject::Ptr object, ObjectType type) const
    {
        if (!object)
        {
            _consoleManager->PrintCriticalHint(_localizationManager->Translate("Storyteller", "Game data is incorrect (object is null), required: ").append(ObjectTypeToString(type)));
            return false;
        }

        if (object->GetObjectType() != type)
        {
            _consoleManager->PrintCriticalHint(_localizationManager->Translate("Storyteller", "Game data is incorrect (object is not of correct type), required: ").append(ObjectTypeToString(type)));
            return false;
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool GameController::ProcessActions(BasicObject::Ptr currentObject, const QuestObject* currentQuestObject, bool& finalReached)
    {
        const auto& questActions = currentQuestObject->GetActions();
        if (!PrintActions(questActions))
        {
            return false;
        }

        int actionIndex = 0;
        while (!actionIndex || (actionIndex >= questActions.size()))
        {
            _consoleManager->PrintInputHint();
            const auto input = _consoleManager->ReadInput();

            try
            {
                actionIndex = std::stoi(input) - 1;
                if (actionIndex >= 0 && actionIndex < questActions.size())
                {
                    currentObject = _gameDocument->GetObject(questActions.at(actionIndex));
                    if (!CheckObject(currentObject, ObjectType::QuestObjectType))
                    {
                        return false;
                    }

                    currentQuestObject = dynamic_cast<QuestObject*>(currentObject.get());
                    finalReached = currentQuestObject->IsFinal();
                }
                else
                {
                    _consoleManager->PrintErrorHint(_localizationManager->Translate("Storyteller", "No action found, try again"));
                }
            }
            catch (const std::exception& e)
            {
                _consoleManager->PrintErrorHint(_localizationManager->Translate("Storyteller", "Cannot recognize action number, try again"));
            }
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool GameController::PrintActions(const std::vector<UUID>& questActions) const
    {
        std::vector<std::string> actionTexts;
        actionTexts.reserve(questActions.size());

        for (auto i = 0; i < questActions.size(); i++)
        {
            const auto object = _gameDocument->GetObject(questActions.at(i));
            if (!CheckObject(object, ObjectType::ActionObjectType))
            {
                return false;
            }

            const auto actionObject = dynamic_cast<ActionObject*>(object.get());
            actionTexts.emplace_back(_localizationManager->Translate(_gameName, actionObject->GetText()));
        }

        _consoleManager->PrintActions(actionTexts);

        return true;
    }
    //--------------------------------------------------------------------------

    void GameController::NewFrame(const QuestObject* currentQuestObject) const
    {
        _consoleManager->StartNewFrame(_gameNameTranslated);
        _consoleManager->PrintMessage(_localizationManager->Translate(_gameName, currentQuestObject->GetText()));
    }
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
}