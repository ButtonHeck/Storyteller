#pragma once

#include "console_manager.h"
#include "Storyteller/pointers.h"
#include "Storyteller/game_document.h"
#include "Storyteller/localization_manager.h"

namespace Storyteller
{
    class GameController
    {
    public:
        GameController(const Ptr<GameDocument> gameDocument, const Ptr<LocalizationManager> localizationManager);

        void Launch();

    private:
        bool MainLoop(UUID& currentUuid);
        void End(UUID& currentUuid);

        bool CheckObject(UUID objectUuid, ObjectType type) const;
        bool ProcessActions(UUID& currentObject, UUID& currentQuestObject, bool& finalReached);
        bool PrintActions(const std::vector<UUID>& questActions) const;
        void NewFrame(UUID& currentQuestObject) const;

    private:
        Ptr<ConsoleManager> const _consoleManager;
        Ptr<GameDocument> const _gameDocument;
        Ptr<LocalizationManager> const _localizationManager;
        const std::string _gameName;
        const std::string _gameNameTranslated;
    };
    //--------------------------------------------------------------------------
}
