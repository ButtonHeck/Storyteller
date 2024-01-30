#include "editor_application.h"
#include "entry_point.h"
#include "storyteller.h"

#include <boost/locale.hpp>
#include <memory>
#include <iostream>

namespace Storyteller
{
    Application* CreateApplication()
    {
        return new EditorApplication();
    }
    //--------------------------------------------------------------------------

    EditorApplication::EditorApplication()
        : _window(nullptr)
        , _ui(nullptr)
    {}
    //--------------------------------------------------------------------------

    std::string EditorApplication::GetApplicationName() const
    {
        return "Storyteller editor";
    }
    //--------------------------------------------------------------------------

    bool EditorApplication::Initialize()
    {
        if (!Application::Initialize())
        {
            return false;
        }

        _localizationManager->AddMessagesDomain(STRTLR_TR_DOMAIN_EDITOR);
        _localizationManager->Build("ru_RU.UTF-8");

        _window.reset(new Window(_localizationManager));
        if (!_window || !_window->Initialize())
        {
            return false;
        }

        _ui.reset(new EditorUi(_window, _localizationManager));
        if (!_ui || !_ui->Initialize())
        {
            return false;
        }

        _window->SetRefreshCallback([&]() {
            _ui->NewFrame();
            _ui->Stylize();
            _ui->BeginDockspace();
            _ui->Compose();
            _ui->EndDockspace();
            _ui->Render();
            _ui->EndFrame();
        });

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
        _window->Shutdown();
    }
    //--------------------------------------------------------------------------
}