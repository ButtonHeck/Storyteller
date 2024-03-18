#include "editor_application.h"
#include "Storyteller/entry_point.h"
#include "Storyteller/storyteller.h"
#include "Storyteller/utils.h"

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

        _window->SetTitle(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Storyteller editor"));
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

        dispatcher.Dispatch<WindowMoveEvent>(STRTLR_BIND(EditorApplication::OnWindowMoveEvent));
        dispatcher.Dispatch<WindowCloseEvent>(STRTLR_BIND(EditorApplication::OnWindowCloseEvent));
        dispatcher.Dispatch<WindowResizeEvent>(STRTLR_BIND(EditorApplication::OnWindowResizeEvent));
        dispatcher.Dispatch<WindowFocusEvent>(STRTLR_BIND(EditorApplication::OnWindowFocusEvent));
        dispatcher.Dispatch<WindowIconifyEvent>(STRTLR_BIND(EditorApplication::OnWindowIconifyEvent));
        dispatcher.Dispatch<WindowFramebufferRefreshEvent>(STRTLR_BIND(EditorApplication::OnWindowFramebufferRefreshEvent));
        dispatcher.Dispatch<WindowFramebufferResizeEvent>(STRTLR_BIND(EditorApplication::OnWindowFramebufferResizeEvent));

        dispatcher.Dispatch<MouseMoveEvent>(STRTLR_BIND(EditorApplication::OnMouseMoveEvent));
        dispatcher.Dispatch<MouseScrollEvent>(STRTLR_BIND(EditorApplication::OnMouseScrollEvent));
        dispatcher.Dispatch<MouseButtonPressEvent>(STRTLR_BIND(EditorApplication::OnMouseButtonPressEvent));
        dispatcher.Dispatch<MouseButtonReleaseEvent>(STRTLR_BIND(EditorApplication::OnMouseButtonReleaseEvent));

        dispatcher.Dispatch<KeyPressEvent>(STRTLR_BIND(EditorApplication::OnKeyPressEvent));
        dispatcher.Dispatch<KeyReleaseEvent>(STRTLR_BIND(EditorApplication::OnKeyReleaseEvent));
        dispatcher.Dispatch<KeyCharEvent>(STRTLR_BIND(EditorApplication::OnKeyCharEvent));
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