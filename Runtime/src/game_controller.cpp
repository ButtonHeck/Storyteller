#include "game_controller.h"
#include "Storyteller/log.h"

namespace Storyteller
{
    GameController::GameController(const Ptr<GameDocument> gameDocument, const Ptr<LocalizationManager> localizationManager)
        : _consoleManager(new ConsoleManager(localizationManager))
        , _gameDocument(gameDocument)
        , _localizationManager(localizationManager)
        , _gameName(_gameDocument->GetGameName())
        , _gameNameTranslated(_localizationManager->Translate(_gameName, _gameName))
    {
        STRTLR_CLIENT_LOG_INFO("GameController: create, game name '{}'", _gameName);
    }
    //--------------------------------------------------------------------------

    void GameController::Launch()
    {
        STRTLR_CLIENT_LOG_INFO("GameController: launched...");

        auto currentUuid = UUID::InvalidUuid;

        const auto noErrors = MainLoop(currentUuid);
        if (noErrors)
        {
            End(currentUuid);
        }
    }
    //--------------------------------------------------------------------------

    bool GameController::MainLoop(UUID& currentUuid)
    {
        STRTLR_CLIENT_LOG_INFO("GameController: main loop started...");

        auto currentObjectUuid = _gameDocument->GetEntryPoint()->GetUuid();
        auto finalReached = false;

        while (!finalReached)
        {
            if (!CheckObject(currentObjectUuid, ObjectType::QuestObjectType))
            {
                return false;
            }

            currentUuid = currentObjectUuid;
            NewFrame(currentUuid);

            if (!ProcessActions(currentObjectUuid, currentUuid, finalReached))
            {
                return false;
            }
        }

        return true;
    }
    //--------------------------------------------------------------------------

    void GameController::End(UUID& currentQuestObject)
    {
        STRTLR_CLIENT_LOG_INFO("GameController: ending...");

        NewFrame(currentQuestObject);
        
        _consoleManager->PrintEndHint();
        _consoleManager->WaitForKeyboardHit();
    }
    //--------------------------------------------------------------------------

    bool GameController::CheckObject(UUID objectUuid, ObjectType type) const
    {
        const auto object = _gameDocument->GetObject(objectUuid);
        if (!object)
        {
            const auto typeString = ObjectTypeToString(type);
            STRTLR_CLIENT_LOG_CRITICAL("GameController: Game data is incorrect (object is null), required: '{}'", typeString);

            _consoleManager->PrintCriticalHint(_localizationManager->Translate(STRTLR_TR_DOMAIN_RUNTIME, "Game data is incorrect (object is null), required: ").append(typeString));
            return false;
        }

        if (object->GetObjectType() != type)
        {
            const auto typeString = ObjectTypeToString(type);
            STRTLR_CLIENT_LOG_CRITICAL("GameController: Game data is incorrect (object '{}' is not of correct type), required: '{}'", object->GetUuid(), typeString);

            _consoleManager->PrintCriticalHint(_localizationManager->Translate(STRTLR_TR_DOMAIN_RUNTIME, "Game data is incorrect (object is not of correct type), required: ").append(typeString));
            return false;
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool GameController::ProcessActions(UUID& currentObject, UUID& currentQuestObject, bool& finalReached)
    {
        const auto qo = dynamic_cast<QuestObject*>(_gameDocument->GetObject(currentQuestObject).get());
        const auto& questActions = qo->GetActions();
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
                actionIndex = std::stoi(input);
                if (actionIndex >= 1 && actionIndex <= questActions.size())
                {
                    currentObject = questActions.at(actionIndex - 1);
                    if (!CheckObject(currentObject, ObjectType::ActionObjectType))
                    {
                        return false;
                    }

                    auto tt = dynamic_cast<ActionObject*>(_gameDocument->GetObject(currentObject).get());
                    currentObject = tt->GetTargetUuid();
                    currentQuestObject = currentObject;
                    finalReached = (dynamic_cast<QuestObject*>(_gameDocument->GetObject(currentQuestObject).get()))->IsFinal();
                }
                else
                {
                    STRTLR_CLIENT_LOG_ERROR("GameController: action index input error, input is '{}', number of actions is '{}'", actionIndex, questActions.size());
                    _consoleManager->PrintErrorHint(_localizationManager->Translate(STRTLR_TR_DOMAIN_RUNTIME, "No action found, try again"));
                }
            }
            catch (const std::exception&)
            {
                STRTLR_CLIENT_LOG_CRITICAL("GameController: cannot recognize action number, input is '{}'", input);
                _consoleManager->PrintErrorHint(_localizationManager->Translate(STRTLR_TR_DOMAIN_RUNTIME, "Cannot recognize action number, try again"));
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
            if (!CheckObject(questActions.at(i), ObjectType::ActionObjectType))
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

    void GameController::NewFrame(UUID& objectUuid) const
    {
        STRTLR_CLIENT_LOG_INFO("GameController: new frame, current uuid is '{}'", objectUuid);

        auto object = _gameDocument->GetObject(objectUuid);
        const auto textObject = dynamic_cast<TextObject*>(object.get());

        _consoleManager->StartNewFrame(_gameNameTranslated);
        _consoleManager->PrintMessage(_localizationManager->Translate(_gameName, textObject->GetText()));
    }
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
}