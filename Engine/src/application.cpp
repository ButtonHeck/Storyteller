#include "application.h"
#include "log.h"
#include "filesystem.h"
#include "config.h"

namespace Storyteller
{
    Application::Application()
        : _config(nullptr)
        , _i18nManager(nullptr)
        , _settings(nullptr)
    {}
    //--------------------------------------------------------------------------

    bool Application::Initialize(const std::string& configPath)
    {
        Filesystem::Initialize();

        _config.reset(new Config());
        _config->Load(configPath.empty() ? Filesystem::GetCurrentPath().append("Storyteller.json") : configPath);

        Log::Initialize(_config->GetLogConfig());

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