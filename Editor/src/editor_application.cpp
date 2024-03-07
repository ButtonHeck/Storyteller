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

            _ui->LoopIteration();

            _window->SwapBuffers();
        }

        _ui->Shutdown();
    }
    //--------------------------------------------------------------------------

    void EditorApplication::OnEvent(Event& event)
    {
        EventDispatcher dispatcher(event);

        dispatcher.Dispatch<WindowMoveEvent>(BIND_EVENT_FUNCTION(EditorApplication::OnWindowMoveEvent));
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FUNCTION(EditorApplication::OnWindowCloseEvent));
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FUNCTION(EditorApplication::OnWindowResizeEvent));
        dispatcher.Dispatch<WindowFocusEvent>(BIND_EVENT_FUNCTION(EditorApplication::OnWindowFocusEvent));
        dispatcher.Dispatch<WindowIconifyEvent>(BIND_EVENT_FUNCTION(EditorApplication::OnWindowIconifyEvent));
        dispatcher.Dispatch<WindowFramebufferRefreshEvent>(BIND_EVENT_FUNCTION(EditorApplication::OnWindowFramebufferRefreshEvent));
        dispatcher.Dispatch<WindowFramebufferResizeEvent>(BIND_EVENT_FUNCTION(EditorApplication::OnWindowFramebufferResizeEvent));

        dispatcher.Dispatch<MouseMoveEvent>(BIND_EVENT_FUNCTION(EditorApplication::OnMouseMoveEvent));
        dispatcher.Dispatch<MouseScrollEvent>(BIND_EVENT_FUNCTION(EditorApplication::OnMouseScrollEvent));
        dispatcher.Dispatch<MouseButtonPressEvent>(BIND_EVENT_FUNCTION(EditorApplication::OnMouseButtonPressEvent));
        dispatcher.Dispatch<MouseButtonReleaseEvent>(BIND_EVENT_FUNCTION(EditorApplication::OnMouseButtonReleaseEvent));

        dispatcher.Dispatch<KeyPressEvent>(BIND_EVENT_FUNCTION(EditorApplication::OnKeyPressEvent));
        dispatcher.Dispatch<KeyReleaseEvent>(BIND_EVENT_FUNCTION(EditorApplication::OnKeyReleaseEvent));
        dispatcher.Dispatch<KeyCharEvent>(BIND_EVENT_FUNCTION(EditorApplication::OnKeyCharEvent));
    }
    //--------------------------------------------------------------------------

    bool EditorApplication::OnWindowMoveEvent(WindowMoveEvent& event)
    {
        return true;
    }
    //--------------------------------------------------------------------------

    bool EditorApplication::OnWindowCloseEvent(WindowCloseEvent& event)
    {
        return _ui->OnWindowCloseEvent(event);
    }
    //--------------------------------------------------------------------------

    bool EditorApplication::OnWindowResizeEvent(WindowResizeEvent& event)
    {
        return true;
    }
    //--------------------------------------------------------------------------

    bool EditorApplication::OnWindowFocusEvent(WindowFocusEvent& event)
    {
        return true;
    }
    //--------------------------------------------------------------------------

    bool EditorApplication::OnWindowIconifyEvent(WindowIconifyEvent& event)
    {
        return true;
    }
    //--------------------------------------------------------------------------

    bool EditorApplication::OnWindowFramebufferRefreshEvent(WindowFramebufferRefreshEvent& event)
    {
        return _ui->OnWindowFramebufferRefreshEvent(event);
    }
    //--------------------------------------------------------------------------

    bool EditorApplication::OnWindowFramebufferResizeEvent(WindowFramebufferResizeEvent& event)
    {
        return true;
    }
    //--------------------------------------------------------------------------

    bool EditorApplication::OnMouseMoveEvent(MouseMoveEvent& event)
    {
        return true;
    }
    //--------------------------------------------------------------------------

    bool EditorApplication::OnMouseScrollEvent(MouseScrollEvent& event)
    {
        return true;
    }
    //--------------------------------------------------------------------------

    bool EditorApplication::OnMouseButtonPressEvent(MouseButtonPressEvent& event)
    {
        return true;
    }
    //--------------------------------------------------------------------------

    bool EditorApplication::OnMouseButtonReleaseEvent(MouseButtonReleaseEvent& event)
    {
        return true;
    }
    //--------------------------------------------------------------------------

    bool EditorApplication::OnKeyPressEvent(KeyPressEvent& event)
    {
        return _ui->OnKeyPressEvent(event);
    }
    //--------------------------------------------------------------------------

    bool EditorApplication::OnKeyReleaseEvent(KeyReleaseEvent& event)
    {
        return true;
    }
    //--------------------------------------------------------------------------

    bool EditorApplication::OnKeyCharEvent(KeyCharEvent& event)
    {
        return true;
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