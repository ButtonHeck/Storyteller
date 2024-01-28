#pragma once

#include "window.h"
#include "game_document_manager.h"
#include "localization_manager.h"

#include <memory>

namespace Storyteller
{
    class EditorUiCompositor
    {
    public:
        typedef std::shared_ptr<EditorUiCompositor> Ptr;

        EditorUiCompositor(Window::Ptr window, LocalizationManager::Ptr localizationManager);

        void Compose();

    private:
        void ComposeMenu(bool& demoWindow);
        void ComposeGameDocumentPanel();
        void ComposePropertiesPanel();
        void ComposeLogPanel();

    private:
        Window::Ptr _window;
        LocalizationManager::Ptr _localizationManager;
        GameDocumentManager::Ptr _gameDocumentManager;
    };
    //--------------------------------------------------------------------------
}
