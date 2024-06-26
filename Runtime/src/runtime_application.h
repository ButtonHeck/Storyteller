#pragma once

#include "game_controller.h"
#include "Storyteller/game_document_manager.h"
#include "Storyteller/pointers.h"
#include "Storyteller/application.h"

namespace Storyteller
{
    class RuntimeApplication : public Application
    {
    public:
        RuntimeApplication();

        std::string GetApplicationName() const override;

        bool Initialize(const std::string& configPath) override;
        void Run() override;

    private:
        void LoadSettings();

    private:
        Ptr<GameDocumentManager> _manager;
        Ptr<GameController> _gameController;
        std::string _gameDocumentPath;
    };
    //--------------------------------------------------------------------------
}