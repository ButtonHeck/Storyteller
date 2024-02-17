#pragma once

#include "localization_manager.h"
#include "settings.h"
#include "pointers.h"
#include "event.h"

namespace Storyteller
{
    class Application
    {
    public:
        Application();

        virtual ~Application() = default;

        virtual std::string GetApplicationName() const = 0;

        virtual bool Initialize();
        virtual void Run() = 0;
        virtual void OnEvent(Event& event) = 0;

    protected:
        Ptr<LocalizationManager> _localizationManager;
        Ptr<Settings> _settings;
    };
    //--------------------------------------------------------------------------

    extern Application* CreateApplication();
}