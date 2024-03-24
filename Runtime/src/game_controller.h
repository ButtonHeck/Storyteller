#pragma once

#include "console_manager.h"
#include "Storyteller/pointers.h"
#include "Storyteller/game_document.h"
#include "Storyteller/i18n_manager.h"

namespace Storyteller
{
    class GameController
    {
    public:
        GameController(const Ptr<GameDocument> gameDocument, const Ptr<I18N::Manager> i18nManager);

        void Launch();

    private:
        bool MainLoop(UUID& currentUuid);
        void End(UUID& currentUuid);

        bool CheckObject(const UUID& objectUuid, ObjectType type) const;
        bool ProcessActions(UUID& currentUuid, bool& finalReached);
        bool PrintActions(const std::vector<UUID>& questActions) const;
        void NewFrame(UUID& currentQuestObject) const;

    private:
        void FillDictionary() const;

    private:
        const Ptr<ConsoleManager> _consoleManager;
        const Ptr<GameDocument> _gameDocument;
        const Ptr<I18N::Manager> _i18nManager;
    };
    //--------------------------------------------------------------------------
}
