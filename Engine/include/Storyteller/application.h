#pragma once

#include "localization_manager.h"
#include "settings.h"
#include "pointers.h"
#include "event.h"
#include "key_event.h"

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

    protected:
        virtual void OnEvent(Event& event) = 0;

        virtual bool OnKeyPressEvent(KeyPressEvent& event) = 0;
        virtual bool OnKeyReleaseEvent(KeyReleaseEvent& event) = 0;
        virtual bool OnKeyCharEvent(KeyCharEvent& event) = 0;

    protected:
        Ptr<LocalizationManager> _localizationManager;
        Ptr<Settings> _settings;
    };
    //--------------------------------------------------------------------------

    extern Application* CreateApplication();
}