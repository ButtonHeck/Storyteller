#include "runtime_application.h"
#include "Storyteller/entry_point.h"
#include "Storyteller/storyteller.h"
#include "Storyteller/log.h"

namespace Storyteller
{
    Application* CreateApplication()
    {
        return new RuntimeApplication();
    }

    RuntimeApplication::RuntimeApplication()
        : Application()
        , _manager(nullptr)
        , _gameController(nullptr)
        , _gameDocumentPath("")
    {}
    //--------------------------------------------------------------------------

    std::string RuntimeApplication::GetApplicationName() const
    {
        return "Storyteller runtime";
    }
    //--------------------------------------------------------------------------

    bool RuntimeApplication::Initialize(const std::string& configPath)
    {
        if (!Application::Initialize(configPath))
        {
            return false;
        }

        _i18nManager->AddMessagesDomain(STRTLR_TR_DOMAIN_RUNTIME);
        _i18nManager->SetLocale(I18N::LocaleRuUTF8Keyword);

        LoadSettings();

        if (!Filesystem::PathExists(_gameDocumentPath))
        {
            STRTLR_CLIENT_LOG_ERROR("RuntimeApplication: cannot find file '{}'", _gameDocumentPath);
            return false;
        }

        _manager.reset(new GameDocumentManager(_i18nManager));
        if (!_manager || !_manager->OpenDocument(std::filesystem::path(_gameDocumentPath)))
        {
            STRTLR_CLIENT_LOG_ERROR("RuntimeApplication: cannot open game document '{}'", _gameDocumentPath);
            return false;
        }

        _gameController.reset(new GameController(_manager->GetDocument(), _i18nManager));

        return true;
    }
    //--------------------------------------------------------------------------

    void RuntimeApplication::Run()
    {
        _gameController->Launch();
    }
    //--------------------------------------------------------------------------

    void RuntimeApplication::LoadSettings()
    {
        _settings->StartLoad();
        _gameDocumentPath = _settings->GetString("GameFile", "game.json");
        _settings->EndLoad();
    }
    //--------------------------------------------------------------------------
}