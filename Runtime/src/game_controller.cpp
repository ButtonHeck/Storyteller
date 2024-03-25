#include "game_controller.h"
#include "Storyteller/log.h"
#include "Storyteller/function_utils.h"

namespace Storyteller
{
    GameController::GameController(const Ptr<GameDocument> gameDocument, const Ptr<I18N::Manager> i18nManager)
        : _consoleManager(CreatePtr<ConsoleManager>(i18nManager))
        , _gameDocument(gameDocument)
        , _i18nManager(i18nManager)
    {
        STRTLR_CLIENT_LOG_INFO("GameController: create, game name '{}'", _gameDocument->GetGameName());

        FillDictionary();
        _i18nManager->AddLocaleChangedCallback(STRTLR_BIND(GameController::FillDictionary));
    }
    //--------------------------------------------------------------------------

    void GameController::Launch()
    {
        STRTLR_CLIENT_LOG_INFO("GameController: launched...");

        auto currentUuid = _gameDocument->GetEntryPoint()->GetUuid();

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

        auto finalReached = false;

        while (!finalReached)
        {
            if (!CheckObject(currentUuid, ObjectType::QuestObjectType))
            {
                return false;
            }

            NewFrame(currentUuid);

            if (!ProcessActions(currentUuid, finalReached))
            {
                return false;
            }
        }

        return true;
    }
    //--------------------------------------------------------------------------

    void GameController::End(UUID& currentUuid)
    {
        STRTLR_CLIENT_LOG_INFO("GameController: ending...");

        NewFrame(currentUuid);
        
        _consoleManager->PrintEndHint();
        _consoleManager->WaitForKeyboardHit();
    }
    //--------------------------------------------------------------------------

    bool GameController::CheckObject(const UUID& objectUuid, ObjectType type) const
    {
        const auto object = _gameDocument->GetObject(objectUuid);
        if (!object)
        {
            const auto typeString = ObjectTypeToString(type);
            STRTLR_CLIENT_LOG_CRITICAL("GameController: Game data is incorrect (object is null), required: '{}'", typeString);

            _consoleManager->PrintCriticalHint(I18N::Translator::Format(_i18nManager->Translation(STRTLR_TR_DOMAIN_RUNTIME, "Game data is incorrect (object is null), required: {1}"), typeString));
            return false;
        }

        if (object->GetObjectType() != type)
        {
            const auto typeString = ObjectTypeToString(type);
            STRTLR_CLIENT_LOG_CRITICAL("GameController: Game data is incorrect (object '{}' is not of correct type), required: '{}'", objectUuid, typeString);

            _consoleManager->PrintCriticalHint(I18N::Translator::Format(_i18nManager->Translation(STRTLR_TR_DOMAIN_RUNTIME, "Game data is incorrect (object is not of correct type), required: {1}"), typeString));
            return false;
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool GameController::ProcessActions(UUID& currentUuid, bool& finalReached)
    {
        const auto questObject = dynamic_cast<QuestObject*>(_gameDocument->GetObject(currentUuid).get());
        const auto& questActions = questObject->GetActions();
        if (!PrintActions(questActions))
        {
            return false;
        }

        int actionNumber = 0;
        while (!actionNumber || (actionNumber > questActions.size()))
        {
            _consoleManager->PrintInputHint();
            const auto input = _consoleManager->ReadInput();

            try
            {
                actionNumber = std::stoi(input);
                if (actionNumber >= 1 && actionNumber <= questActions.size())
                {
                    const auto& chosenActionUuid = questActions.at(actionNumber - 1);
                    if (!CheckObject(chosenActionUuid, ObjectType::ActionObjectType))
                    {
                        return false;
                    }

                    const auto chosenActionObject = dynamic_cast<ActionObject*>(_gameDocument->GetObject(chosenActionUuid).get());
                    currentUuid = chosenActionObject->GetTargetUuid();
                    finalReached = (dynamic_cast<QuestObject*>(_gameDocument->GetObject(currentUuid).get()))->IsFinal();
                }
                else
                {
                    STRTLR_CLIENT_LOG_ERROR("GameController: action index input error, input is '{}', number of actions is '{}'", actionNumber, questActions.size());
                    _consoleManager->PrintErrorHint(_i18nManager->Translation(STRTLR_TR_DOMAIN_RUNTIME, "No action found, try again"));
                }
            }
            catch (const std::exception&)
            {
                STRTLR_CLIENT_LOG_CRITICAL("GameController: cannot recognize action number, input is '{}'", input);
                _consoleManager->PrintErrorHint(_i18nManager->Translation(STRTLR_TR_DOMAIN_RUNTIME, "Cannot recognize action number, try again"));
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
            actionTexts.emplace_back(_i18nManager->Translation(_gameDocument->GetDomainName(), actionObject->GetText()));
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

        _consoleManager->StartNewFrame(_i18nManager->Translation(_gameDocument->GetDomainName(), _gameDocument->GetGameName()));
        _consoleManager->PrintMessage(_i18nManager->Translation(_gameDocument->GetDomainName(), textObject->GetText()));
    }
    //--------------------------------------------------------------------------

    void GameController::FillDictionary() const
    {
        _i18nManager->Translate(STRTLR_TR_DOMAIN_RUNTIME, "Game data is incorrect (object is null), required: {1}");
        _i18nManager->Translate(STRTLR_TR_DOMAIN_RUNTIME, "Game data is incorrect (object is not of correct type), required: {1}");
        _i18nManager->Translate(STRTLR_TR_DOMAIN_RUNTIME, "No action found, try again");
        _i18nManager->Translate(STRTLR_TR_DOMAIN_RUNTIME, "Cannot recognize action number, try again");
    }
    //--------------------------------------------------------------------------
    //--------------------------------------------------------------------------
}