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

        _settings.reset(new Settings(GetApplicationName()));

        return true;
    }
    //--------------------------------------------------------------------------
}