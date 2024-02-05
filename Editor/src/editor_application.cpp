#include "editor_application.h"
#include "Storyteller/entry_point.h"
#include "Storyteller/storyteller.h"

#include <boost/locale.hpp>

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
        _localizationManager->Build("ru_RU.UTF-8");

        _ui.reset(new EditorUi(_window, _localizationManager));
        if (!_ui || !_ui->Initialize())
        {
            return false;
        }

        _window->SetTitle(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, GetApplicationName()));
        _window->SetRefreshCallback([&]() {
            _ui->NewFrame();
            _ui->Stylize();
            _ui->BeginDockspace();
            _ui->Compose();
            _ui->EndDockspace();
            _ui->Render();
            _ui->EndFrame();
        });

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