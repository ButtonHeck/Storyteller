#include "application.h"
#include "log.h"
#include "filesystem.h"

namespace Storyteller
{
    Application::Application()
        : _localizationManager(nullptr)
        , _settings(nullptr)
    {}
    //--------------------------------------------------------------------------

    bool Application::Initialize()
    {
        Filesystem::Initialize();
        Log::Initialize();

        _localizationManager.reset(new LocalizationManager());
        _localizationManager->AddMessagesDomain(STRTLR_TR_DOMAIN_ENGINE);

        _localizationManager->Translate(STRTLR_TR_DOMAIN_ENGINE, "Quest object");
        _localizationManager->Translate(STRTLR_TR_DOMAIN_ENGINE, "Action object");

        _settings.reset(new Settings(GetApplicationName()));

        return true;
    }
    //--------------------------------------------------------------------------
}