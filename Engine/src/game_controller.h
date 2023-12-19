#pragma once

#include "game_document.h"
#include "console_manager.h"
#include "localization_manager.h"

namespace Storyteller
{
    class GameController
    {
    public:
        typedef std::shared_ptr<GameController> Ptr;

        GameController(const GameDocument::Ptr gameDocument, const LocalizationManager::Ptr localizationManager);

        void Launch();

    private:
        bool MainLoop(const QuestObject* currentQuestObject);
        void End(const QuestObject* currentQuestObject);

        bool CheckObject(const BasicObject::Ptr object, ObjectType type) const;
        bool ProcessActions(BasicObject::Ptr currentObject, const QuestObject* currentQuestObject, bool& finalReached);
        bool PrintActions(const std::vector<UUID>& questActions) const;
        void NewFrame(const QuestObject* currentQuestObject) const;

    private:
        ConsoleManager::Ptr const _consoleManager;
        GameDocument::Ptr const _gameDocument;
        LocalizationManager::Ptr const _localizationManager;
        const std::string _gameName;
        const std::string _gameNameTranslated;
    };
    //--------------------------------------------------------------------------
}
