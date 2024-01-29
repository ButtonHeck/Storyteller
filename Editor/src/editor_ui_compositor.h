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

            //TODO: save/load to editor config
            bool logPanel = true;
        };

    private:
        void ComposeMenu();
        void ComposeMenuFile();
        void ComposeMenuView();
        void ComposeMenuItemNew();
        void ComposeMenuItemOpen();
        void ComposeMenuItemSave();
        void ComposeMenuItemSaveAs();
        void ComposeMenuItemQuit();
        void ComposeMenuItemDemoWindow();
        void ComposeMenuItemLog();

        void ComposeGameDocumentPanel();
        void ComposeGameDocumentPanelGame();
        void ComposeGameDocumentPanelObjectsManagement();
        void ComposeGameDocumentPanelFilters();
        void ComposeGameDocumentPanelFilterCheckbox(ObjectType objectType, bool& filterState);
        void ComposeGameDocumentPanelObjectsTable();

        void ComposePropertiesPanel();
        void ComposePropertiesPanelName(BasicObject::Ptr selectedObject);
        void ComposePropertiesPanelTexts(BasicObject::Ptr selectedObject);
        void ComposePropertiesPanelQuestObject(BasicObject::Ptr selectedObject);
        void ComposePropertiesPanelActionObject(BasicObject::Ptr selectedObject);

        void ComposeLogPanel();

    private:
        Window::Ptr _window;
        LocalizationManager::Ptr _localizationManager;
        GameDocumentManager::Ptr _gameDocumentManager;
        UiComponentsState _state;
    };
    //--------------------------------------------------------------------------
}
