#pragma once

#include "console_manager.h"
#include "Storyteller/pointers.h"
#include "Storyteller/game_document_manager.h"
#include "Storyteller/localization_manager.h"

namespace Storyteller
{
    class GameController
    {
    public:
        GameController(const Ptr<GameDocumentManager> gameDocumentManager, const Ptr<LocalizationManager> localizationManager);

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
        Ptr<GameDocumentManager> const _gameDocumentManager;
        Ptr<LocalizationManager> const _localizationManager;
        const std::string _gameName;
        const std::string _gameNameTranslated;
    };
    //--------------------------------------------------------------------------
}
