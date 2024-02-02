#pragma once

#include "window.h"
#include "game_document_manager.h"
#include "localization_manager.h"
#include "settings.h"
#include "pointers.h"

namespace Storyteller
{
    class EditorUiCompositor
    {
    public:
        EditorUiCompositor(Ptr<Window> window, Ptr<LocalizationManager> localizationManager);

        void Compose();

        void SaveSettings(Ptr<Settings> settings) const;
        void LoadSettings(Ptr<Settings> settings);

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
        void ComposePropertiesPanelName(Ptr<BasicObject> selectedObject);
        void ComposePropertiesPanelTexts(Ptr<BasicObject> selectedObject);
        void ComposePropertiesPanelQuestObject(Ptr<BasicObject> selectedObject);
        void ComposePropertiesPanelActionObject(Ptr<BasicObject> selectedObject);

        void ComposeLogPanel();

    private:
        Ptr<Window> _window;
        Ptr<LocalizationManager> _localizationManager;
        Ptr<GameDocumentManager> _gameDocumentManager;
        UiComponentsState _state;
    };
    //--------------------------------------------------------------------------
}
