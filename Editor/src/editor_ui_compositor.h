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
        struct UiComponentsState
        {
            bool demoWindow = false;
            int selectedTypeIndex = 0;
            bool questObjectFilter = true;
            bool actionObjectFilter = true;
            int selectedChildActionIndex = 0;
            int selectedActionIndex = 0;
            int selectedQuestIndex = 0;
        };

    private:
        void ComposeMenu();
        void ComposeGameDocumentPanel();
        void ComposePropertiesPanel();
        void ComposeLogPanel();

    private:
        Window::Ptr _window;
        LocalizationManager::Ptr _localizationManager;
        GameDocumentManager::Ptr _gameDocumentManager;
        UiComponentsState _state;
    };
    //--------------------------------------------------------------------------
}
