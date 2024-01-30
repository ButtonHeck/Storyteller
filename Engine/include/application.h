#pragma once

#include "localization_manager.h"
#include "settings.h"

namespace Storyteller
{
    class Application
    {
    public:
        Application();

        virtual ~Application() = default;

        virtual std::string GetApplicationName() const = 0;

        virtual bool Initialize();
        virtual void Run();

    protected:
        LocalizationManager::Ptr _localizationManager;
        Settings::Ptr _settings;
    };
    //--------------------------------------------------------------------------

    extern Application* CreateApplication();
}