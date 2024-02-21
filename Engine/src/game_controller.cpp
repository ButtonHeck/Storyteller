#include "game_controller.h"
#include "log.h"

namespace Storyteller
{
    GameController::GameController(const Ptr<GameDocument> gameDocument, const Ptr<LocalizationManager> localizationManager)
        : _consoleManager(new ConsoleManager(localizationManager))
        , _gameDocument(gameDocument)
        , _localizationManager(localizationManager)
        , _gameName(_gameDocument->GetGameName())
        , _gameNameTranslated(_localizationManager->Translate(_gameName, _gameName))
    {
        STRTLR_CORE_LOG_INFO("GameController: create, game name '{}'", _gameName);
    }
    //--------------------------------------------------------------------------

    void GameController::Launch()
    {
        STRTLR_CORE_LOG_INFO("GameController: launched...");

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
        STRTLR_CORE_LOG_INFO("GameController: main loop started...");

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
        STRTLR_CORE_LOG_INFO("GameController: ending...");

        NewFrame(currentQuestObject);
        
        _consoleManager->PrintEndHint();
        _consoleManager->WaitForKeyboardHit();
    }
    //--------------------------------------------------------------------------

    bool GameController::CheckObject(const Ptr<BasicObject> object, ObjectType type) const
    {
        if (!object)
        {
            const auto typeString = ObjectTypeToString(type);
            STRTLR_CORE_LOG_CRITICAL("GameController: Game data is incorrect (object is null), required: '{}'", typeString);

            _consoleManager->PrintCriticalHint(_localizationManager->Translate("Storyteller", "Game data is incorrect (object is null), required: ").append(typeString));
            return false;
        }

        if (object->GetObjectType() != type)
        {
            const auto typeString = ObjectTypeToString(type);
            STRTLR_CORE_LOG_CRITICAL("GameController: Game data is incorrect (object '{}' is not of correct type), required: '{}'", object->GetUuid(), typeString);

            _consoleManager->PrintCriticalHint(_localizationManager->Translate("Storyteller", "Game data is incorrect (object is not of correct type), required: ").append(typeString));
            return false;
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool GameController::ProcessActions(Ptr<BasicObject> currentObject, const QuestObject* currentQuestObject, bool& finalReached)
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
                    STRTLR_CORE_LOG_ERROR("GameController: action index input error, input is '{}', number of actions is '{}'", actionIndex, questActions.size());
                    _consoleManager->PrintErrorHint(_localizationManager->Translate("Storyteller", "No action found, try again"));
                }
            }
            catch (const std::exception&)
            {
                STRTLR_CORE_LOG_CRITICAL("GameController: cannot recognize action number, input is '{}'", input);
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
        STRTLR_CORE_LOG_INFO("GameController: new frame, current uuid is '{}'", currentQuestObject->GetUuid());

        _consoleManager->StartNewFrame(_gameNameTranslated);
        _consoleManager->PrintMessage(_localizationManager->Translate(_gameName, currentQuestObject->GetText()));
    }
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
}