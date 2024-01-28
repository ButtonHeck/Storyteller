#pragma once

#include "game_document_manager.h"
#include "localization_manager.h"
#include "window.h"

#include <memory>

namespace Storyteller
{
    class EditorUi
    {
    public:
        typedef std::shared_ptr<EditorUi> Ptr;

        EditorUi(Window::Ptr window, LocalizationManager::Ptr localizationManager);

        bool Initialize();
        void NewFrame();
        void Stylize();
        void BeginDockspace();
        void Compose();
        void EndDockspace();
        void Render();
        void EndFrame();
        void Shutdown();

    private:
        void ComposeMenu(bool& demoWindow);
        void ComposeGameDocumentPanel();
        void ComposePropertiesPanel();
        void ComposeLogPanel();

        void AddDefaultFont();

    private:
        Window::Ptr _window;
        LocalizationManager::Ptr _localizationManager;
        GameDocumentManager::Ptr _gameDocumentManager;
    };
    //--------------------------------------------------------------------------
}
