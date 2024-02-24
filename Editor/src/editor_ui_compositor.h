#pragma once

#include "Storyteller/window.h"
#include "Storyteller/game_document_manager.h"
#include "Storyteller/localization_manager.h"
#include "Storyteller/settings.h"
#include "Storyteller/pointers.h"
#include "Storyteller/key_event.h"
#include "Storyteller/window_event.h"

#include <list>

namespace Storyteller
{
    class EditorUiCompositor
    {
    public:
        EditorUiCompositor(Ptr<Window> window, Ptr<LocalizationManager> localizationManager);

        void Compose();

        bool OnKeyPressEvent(KeyPressEvent& event);
        bool OnWindowCloseEvent(WindowCloseEvent& event);

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

        struct UiPopupsState
        {
            bool newDocument = false;
            bool quit = false;

            bool warningMessage = false;
            std::string warningMessageText = "";

            bool openDocument = false;
            std::string openDocumentFile = "";
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

        void ComposePopups();

    private:
        void PopupNewDocument();
        void PopupQuit();
        void PopupWarningMessage();
        void PopupOpenDocument();

        void SaveDocument();
        void SaveAsDocument();
        void OpenDocument(const std::string& filename);
        void SwitchLogWindowVisibility();
        void SwitchFullscreen();

    private:
        Ptr<Window> _window;
        Ptr<LocalizationManager> _localizationManager;
        Ptr<GameDocumentManager> _gameDocumentManager;
        UiComponentsState _state;
        UiPopupsState _popups;
        std::list<std::string> _recentList;
    };
    //--------------------------------------------------------------------------
}
