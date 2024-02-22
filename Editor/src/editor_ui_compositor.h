#pragma once

#include "Storyteller/window.h"
#include "Storyteller/game_document_manager.h"
#include "Storyteller/localization_manager.h"
#include "Storyteller/settings.h"
#include "Storyteller/pointers.h"
#include "Storyteller/key_event.h"

#include <list>

namespace Storyteller
{
    class EditorUiCompositor
    {
    public:
        EditorUiCompositor(Ptr<Window> window, Ptr<LocalizationManager> localizationManager);

        void Compose();

        bool ReadyToClose() const;
        bool OnKeyPressEvent(KeyPressEvent& event);

        void SaveSettings(Ptr<Settings> settings) const;
        void LoadSettings(Ptr<Settings> settings);

    private:
        struct UiComponentsState
        {
            bool demoWindow = false;
            bool questObjectFilter = true;
            bool actionObjectFilter = true;
            int selectedChildActionIndex = 0;
            int selectedActionIndex = 0;
            int selectedQuestIndex = 0;
            bool logPanel = true;
            bool logAutoscroll = false;
        };

    private:
        void ComposeDefaultPanelsLayout();

        void ComposeMenu();
        void ComposeMenuFile();
        void ComposeMenuView();
        void ComposeMenuItemNew();
        void ComposeMenuItemOpen();
        void ComposeMenuItemOpenRecent();
        void ComposeMenuItemSave();
        void ComposeMenuItemSaveAs();
        void ComposeMenuItemQuit();
        void ComposeMenuItemDemoWindow();
        void ComposeMenuItemLog();
        void ComposeMenuItemFullscreen();

        void ComposeGameDocumentPanel();
        void ComposeGameDocumentPanelGame();
        void ComposeGameDocumentPanelObjectsManagement();
        void ComposeGameDocumentPanelFilterCheckbox(ObjectType objectType, bool& filterState);
        void ComposeGameDocumentPanelObjectsTable();

        void ComposePropertiesPanel();
        void ComposePropertiesPanelName(Ptr<BasicObject> selectedObject);
        void ComposePropertiesPanelTexts(Ptr<BasicObject> selectedObject);
        void ComposePropertiesPanelQuestObject(Ptr<BasicObject> selectedObject);
        void ComposePropertiesPanelActionObject(Ptr<BasicObject> selectedObject);

        void ComposeLogPanel();

    private:
        void NewDocument();
        void Exit();
        void OpenDocument();
        void SaveDocument();
        void SaveAsDocument();
        void SwitchLogWindowVisibility();
        void SwitchFullscreen();

    private:
        Ptr<Window> _window;
        Ptr<LocalizationManager> _localizationManager;
        Ptr<GameDocumentManager> _gameDocumentManager;
        UiComponentsState _state;
        std::list<std::string> _recentList;
    };
    //--------------------------------------------------------------------------
}
