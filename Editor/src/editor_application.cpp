#include "editor_application.h"
#include "Storyteller/entry_point.h"
#include "Storyteller/storyteller.h"

namespace Storyteller
{
    Application* CreateApplication()
    {
        return new EditorApplication();
    }
    //--------------------------------------------------------------------------

    EditorApplication::EditorApplication()
        : WindowApplication()
        , _ui(nullptr)
    {}
    //--------------------------------------------------------------------------

    EditorApplication::~EditorApplication()
    {
        SaveSettings();
    }
    //--------------------------------------------------------------------------

    std::string EditorApplication::GetApplicationName() const
    {
        return "Storyteller editor";
    }
    //--------------------------------------------------------------------------

    bool EditorApplication::Initialize()
    {
        if (!WindowApplication::Initialize())
        {
            return false;
        }

        _localizationManager->AddMessagesDomain("StorytellerEditor");
        _localizationManager->SetLocale("ru_RU.UTF-8");

        _ui.reset(new EditorUi(_window, _localizationManager));
        if (!_ui || !_ui->Initialize())
        {
            return false;
        }

        _window->SetTitle(_localizationManager->Translate("StorytellerEditor", "Storyteller editor"));
        _window->SetEventCallback(BIND_EVENT_FUNCTION(EditorApplication::OnEvent));

        LoadSettings();

        return true;
    }
    //--------------------------------------------------------------------------

    void EditorApplication::Run()
    {
        while (!_window->ShouldClose())
        {
            _window->ProcessEvents();

            _ui->NewFrame();
            _ui->Stylize();
            _ui->BeginDockspace();
            _ui->Compose();
            _ui->EndDockspace();
            _ui->Render();
            _ui->EndFrame();

            _window->SwapBuffers();
        }

        _ui->Shutdown();
    }
    //--------------------------------------------------------------------------

    void EditorApplication::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);
        dispatcher.Dispatch<WindowCloseEvent>([&](WindowCloseEvent&) {
            if (!_ui->ReadyToClose())
            {
                _window->SetShouldClose(false);
            }

            return true;
            }
        );

        dispatcher.Dispatch<WindowFramebufferRefreshEvent>([&](WindowFramebufferRefreshEvent&) {
            _ui->NewFrame();
            _ui->Stylize();
            _ui->BeginDockspace();
            _ui->Compose();
            _ui->EndDockspace();
            _ui->Render();
            _ui->EndFrame();

            return true;
            }
        );
    }
    //--------------------------------------------------------------------------

    void EditorApplication::SaveSettings() const
    {
        _settings->StartSave();
        _ui->SaveSettings(_settings);
        _window->SaveSettings(_settings);
        _settings->EndSave();
    }
    //--------------------------------------------------------------------------

    void EditorApplication::LoadSettings()
    {
        _settings->StartLoad();
        _ui->LoadSettings(_settings);
        _window->LoadSettings(_settings);
        _settings->EndLoad();
    }
    //--------------------------------------------------------------------------
}