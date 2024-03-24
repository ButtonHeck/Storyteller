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

        _localizationManager->AddMessagesDomain(STRTLR_TR_DOMAIN_EDITOR);

        _ui.reset(new EditorUi(_window, _localizationManager));
        if (!_ui || !_ui->Initialize())
        {
            return false;
        }

        _window->SetTitle(GetApplicationName());
        _window->SetEventCallback(STRTLR_BIND(EditorApplication::OnEvent));

        LoadSettings();

        return true;
    }
    //--------------------------------------------------------------------------

    void EditorApplication::Run()
    {
        while (!_window->ShouldClose())
        {
            _window->ProcessEvents();

            _ui->LoopIteration();

            _window->SwapBuffers();
        }

        _ui->Shutdown();
    }
    //--------------------------------------------------------------------------

    void EditorApplication::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<WindowCloseEvent>(STRTLR_BIND(EditorApplication::OnWindowCloseEvent));
        dispatcher.Dispatch<WindowFramebufferRefreshEvent>(STRTLR_BIND(EditorApplication::OnWindowFramebufferRefreshEvent));

        dispatcher.Dispatch<KeyPressEvent>(STRTLR_BIND(EditorApplication::OnKeyPressEvent));
    }
    //--------------------------------------------------------------------------

    bool EditorApplication::OnWindowCloseEvent(WindowCloseEvent& event)
    {
        return _ui->OnWindowCloseEvent(event);
    }
    //--------------------------------------------------------------------------

    bool EditorApplication::OnWindowFramebufferRefreshEvent(WindowFramebufferRefreshEvent& event)
    {
        return _ui->OnWindowFramebufferRefreshEvent(event);
    }
    //--------------------------------------------------------------------------

    bool EditorApplication::OnKeyPressEvent(KeyPressEvent& event)
    {
        return _ui->OnKeyPressEvent(event);
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