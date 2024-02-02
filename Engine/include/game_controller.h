#pragma once

#include "pointers.h"
#include "game_document.h"
#include "console_manager.h"
#include "localization_manager.h"

namespace Storyteller
{
    class GameController
    {
    public:
        GameController(const Ptr<GameDocument> gameDocument, const Ptr<LocalizationManager> localizationManager);

        void Launch();

    private:
        bool MainLoop(const QuestObject* currentQuestObject);
        void End(const QuestObject* currentQuestObject);

        bool CheckObject(const Ptr<BasicObject> object, ObjectType type) const;
        bool ProcessActions(Ptr<BasicObject> currentObject, const QuestObject* currentQuestObject, bool& finalReached);
        bool PrintActions(const std::vector<UUID>& questActions) const;
        void NewFrame(const QuestObject* currentQuestObject) const;

    private:
        Ptr<ConsoleManager> const _consoleManager;
        Ptr<GameDocument> const _gameDocument;
        Ptr<LocalizationManager> const _localizationManager;
        const std::string _gameName;
        const std::string _gameNameTranslated;
    };
    //--------------------------------------------------------------------------
}
