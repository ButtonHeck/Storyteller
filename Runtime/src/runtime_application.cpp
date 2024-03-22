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

    RuntimeApplication::~RuntimeApplication()
    {}
    //--------------------------------------------------------------------------

    std::string RuntimeApplication::GetApplicationName() const
    {
        return "Storyteller runtime";
    }
    //--------------------------------------------------------------------------

    bool RuntimeApplication::Initialize()
    {
        if (!Application::Initialize())
        {
            return false;
        }

        _localizationManager->AddMessagesDomain(STRTLR_TR_DOMAIN_RUNTIME);
        _localizationManager->SetLocale(LocalizationManager::LocaleRuUTF8Keyword);

        _manager.reset(new GameDocumentManager(_localizationManager));
        _manager->OpenDocument(std::filesystem::path(u8"C:\\workspace\\Storyteller\\build\\MyGame\\MyGame.json"));

        _gameController.reset(new GameController(_manager->GetDocument(), _localizationManager));

        return true;
    }
    //--------------------------------------------------------------------------

    void RuntimeApplication::Run()
    {
        _gameController->Launch();
    }
    //--------------------------------------------------------------------------

    void RuntimeApplication::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<KeyPressEvent>(STRTLR_BIND(RuntimeApplication::OnKeyPressEvent));
        dispatcher.Dispatch<KeyReleaseEvent>(STRTLR_BIND(RuntimeApplication::OnKeyReleaseEvent));
        dispatcher.Dispatch<KeyCharEvent>(STRTLR_BIND(RuntimeApplication::OnKeyCharEvent));
    }
    //--------------------------------------------------------------------------

    bool RuntimeApplication::OnKeyPressEvent(KeyPressEvent& event)
    {
        return true;
    }
    //--------------------------------------------------------------------------

    bool RuntimeApplication::OnKeyReleaseEvent(KeyReleaseEvent& event)
    {
        return true;
    }
    //--------------------------------------------------------------------------

    bool RuntimeApplication::OnKeyCharEvent(KeyCharEvent& event)
    {
        return true;
    }
    //--------------------------------------------------------------------------
}