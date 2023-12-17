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

        void Start();

    private:
        ConsoleManager::Ptr const _consoleManager;
        GameDocument::Ptr const _gameDocument;
        LocalizationManager::Ptr const _localizationManager;
    };
    //--------------------------------------------------------------------------
}
