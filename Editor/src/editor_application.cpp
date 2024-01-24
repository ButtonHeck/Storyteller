#include "editor_application.h"
#include "entry_point.h"
#include "storyteller.h"

#include <boost/locale.hpp>
#include <memory>
#include <iostream>
#include <thread>

#define STORYTELLER_DOMAIN "Storyteller"
#define EDITOR_DOMAIN "StorytellerEditor"

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

    bool EditorApplication::Initialize()
    {
        if (!Application::Initialize())
        {
            return false;
        }

        _localizationManager->AddMessagesDomain(EDITOR_DOMAIN);
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

        return true;
    }
    //--------------------------------------------------------------------------

    void EditorApplication::Run()
    {
        while (!_window->ShouldClose())
        {
            _window->ProcessEvents();

            _ui->Prepare();
            _ui->Compose();
            _ui->Render();

            _window->SwapBuffers();
        }

        _ui->Shutdown();
        _window->Shutdown();
    }
    //--------------------------------------------------------------------------
}