#include "runtime_application.h"
#include "Storyteller/entry_point.h"
#include "Storyteller/storyteller.h"

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

        _manager.reset(new GameDocumentManager(_i18nManager));
        _manager->OpenDocument(std::filesystem::path(u8"C:\\workspace\\STORYTELLER\\MyGame\\MyGame.json"));

        _gameController.reset(new GameController(_manager->GetDocument(), _i18nManager));

        return true;
    }
    //--------------------------------------------------------------------------

    void RuntimeApplication::Run()
    {
        _gameController->Launch();
    }
    //--------------------------------------------------------------------------
}