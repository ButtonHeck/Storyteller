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

        bool Initialize() override;
        void Run() override;

    private:
        Ptr<GameDocumentManager> _manager;
        Ptr<GameController> _gameController;
    };
    //--------------------------------------------------------------------------
}