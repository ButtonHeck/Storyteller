#pragma once

#include "localization_manager.h"

namespace Storyteller
{
    class Application
    {
    public:
        Application() = default;

        virtual ~Application() = default;

        virtual bool Initialize();
        virtual void Run();

    protected:
        LocalizationManager::Ptr _localizationManager;
    };
    //--------------------------------------------------------------------------

    extern Application* CreateApplication();
}