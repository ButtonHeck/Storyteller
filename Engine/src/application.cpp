#include "application.h"
#include "log.h"
#include "filesystem.h"

namespace Storyteller
{
    Application::Application()
        : _i18nManager(nullptr)
        , _settings(nullptr)
    {}
    //--------------------------------------------------------------------------

    bool Application::Initialize()
    {
        Filesystem::Initialize();
        Log::Initialize();

        _i18nManager.reset(new I18N::Manager(I18N::LocaleEnUTF8Keyword));
        _i18nManager->AddMessagesDomain(STRTLR_TR_DOMAIN_ENGINE);

        _i18nManager->AddLocaleChangedCallback([this]() {
            _i18nManager->Translate(STRTLR_TR_DOMAIN_ENGINE, "Quest object");
            _i18nManager->Translate(STRTLR_TR_DOMAIN_ENGINE, "Action object");
        });

        _settings.reset(new Settings(GetApplicationName()));

        return true;
    }
    //--------------------------------------------------------------------------
}