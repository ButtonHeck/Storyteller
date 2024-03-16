#include "editor_ui_compositor.h"
#include "ui_utils.h"
#include "icons_font.h"
#include "Storyteller/log.h"
#include "Storyteller/dialogs.h"
#include "Storyteller/filesystem.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>

namespace Storyteller
{
    EditorUiCompositor::EditorUiCompositor(Ptr<Window> window, Ptr<LocalizationManager> localizationManager)
        : _window(window)
        , _localizationManager(localizationManager)
        , _gameDocumentManager(new GameDocumentManager())
        , _translationsDict(_localizationManager->GetDictionary(STRTLR_TR_DOMAIN_EDITOR))
    {
        assert(_translationsDict);

        _translationsDict->Add("Open", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Open"));
        _translationsDict->Add("Quit", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Quit"));
        _translationsDict->Add("Log", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Log"));
        _translationsDict->Add("Name", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Name"));
        _translationsDict->Add("Actions", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Actions"));
        _translationsDict->Add("Find object", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Find object"));
        _translationsDict->Add("New document", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "New document"));
        _translationsDict->Add("Open document", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Open document"));
        _translationsDict->Add("Yes", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Yes"));
        _translationsDict->Add("No", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "No"));
        _translationsDict->Add("Warning", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Warning"));
        _translationsDict->Add("File", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "File"));
        _translationsDict->Add("View", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "View"));
        _translationsDict->Add("New", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "New"));
        _translationsDict->Add("Open recent", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Open recent"));
        _translationsDict->Add("Clear", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Clear"));
        _translationsDict->Add("Save", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Save"));
        _translationsDict->Add("Save as...", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Save as..."));
        _translationsDict->Add("Fullscreen", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Fullscreen"));
        _translationsDict->Add("Untitled document", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Untitled document"));
        _translationsDict->Add("Game document", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Game document"));
        _translationsDict->Add("Game name cannot be empty!", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Game name cannot be empty!"));
        _translationsDict->Add("Create translations file", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Create translations file"));
        _translationsDict->Add("Save translations", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Save translations"));
        _translationsDict->Add("Objects management", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Objects management"));
        _translationsDict->Add("Add object", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Add object"));
        _translationsDict->Add("Visibility filters", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Visibility filters"));
        _translationsDict->Add("Objects", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Objects"));
        _translationsDict->Add("Type", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Type"));
        _translationsDict->Add("UUID", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "UUID"));
        _translationsDict->Add("Delete object", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Delete object"));
        _translationsDict->Add("Properties", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Properties"));
        _translationsDict->Add("Object name cannot be empty!", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Object name cannot be empty!"));
        _translationsDict->Add("Object name already exists!", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Object name already exists!"));
        _translationsDict->Add("Source text", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Source text"));
        _translationsDict->Add("Translation", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Translation"));
        _translationsDict->Add("Entry point", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Entry point"));
        _translationsDict->Add("Final", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Final"));
        _translationsDict->Add("Add action to object", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Add action to object"));
        _translationsDict->Add("Action name", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Action name"));
        _translationsDict->Add("Move action up", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Move action up"));
        _translationsDict->Add("Move action down", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Move action down"));
        _translationsDict->Add("Objects's actions", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Objects's actions"));
        _translationsDict->Add("Text", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Text"));
        _translationsDict->Add("Remove action from object", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Remove action from object"));
        _translationsDict->Add("Find action object", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Find action object"));
        _translationsDict->Add("Clear target", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Clear target"));
        _translationsDict->Add("Set target", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Set target"));
        _translationsDict->Add("Quest object name", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Quest object name"));
        _translationsDict->Add("Current target name: ", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Current target name: "));
        _translationsDict->Add("Not set or does not exist", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Not set or does not exist"));
        _translationsDict->Add("Scroll to end", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Scroll to end"));
        _translationsDict->Add("Autoscroll to end", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Autoscroll to end"));
        _translationsDict->Add("You have unsaved changes, create new document anyway?", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "You have unsaved changes, create new document anyway?"));
        _translationsDict->Add("You have unsaved changes, quit anyway?", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "You have unsaved changes, quit anyway?"));
        _translationsDict->Add("Ok", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Ok"));
        _translationsDict->Add("You have unsaved changes, open other document anyway?", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "You have unsaved changes, open other document anyway?"));
        _translationsDict->Add("Save document", _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Save document"));
        _translationsDict->Add("The selected file is missing or damaged", "Popup message", _localizationManager->TranslateCtx(STRTLR_TR_DOMAIN_EDITOR, "The selected file is missing or damaged", "Popup message"));
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::Compose()
    {
        if (!Filesystem::PathExists(Filesystem::GetCurrentPath().append(ImGui::GetIO().IniFilename)))
        {
            ComposeDefaultPanelsLayout();
        }

        ComposeMenu();
        ComposeGameDocumentPanel();
        ComposePropertiesPanel();

        if (_state.logPanel)
        {
            ComposeLogPanel();
        }

        if (_state.demoWindow)
        {
            ImGui::ShowDemoWindow();
        }

        ComposePopups();
    }
    //--------------------------------------------------------------------------

    bool EditorUiCompositor::OnKeyPressEvent(KeyPressEvent& event)
    {
        const auto keyCode = event.GetKeyCode();
        const auto mods = event.GetMods();

        if (event.IsRepeat())
        {
            return true;
        }

        if (keyCode == Key::Q && mods & Mode::Ctrl)
        {
            _popups.quit = true;
        }
        else if (keyCode == Key::N && mods & Mode::Ctrl)
        {
            _popups.newDocument = true;
        }
        else if (keyCode == Key::O && mods & Mode::Ctrl)
        {
            _popups.openDocument = true;
            _popups.openDocumentFile = "";
        }
        else if (keyCode == Key::S && mods & Mode::Ctrl)
        {
            if (mods & Mode::Shift)
            {
                SaveAsDocument();
            }
            else
            {
                SaveDocument();
            }
        }
        else if (keyCode == Key::L && mods & Mode::Ctrl)
        {
            SwitchLogWindowVisibility();
        }
        else if (keyCode == Key::Enter && mods & Mode::Alt)
        {
            SwitchFullscreen();
        }

        return true;
    }
    //--------------------------------------------------------------------------

    bool EditorUiCompositor::OnWindowCloseEvent(WindowCloseEvent& event)
    {
        _popups.quit = true;
        _window->SetShouldClose(false);
        return true;
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::SaveSettings(Ptr<Settings> settings) const
    {
        settings->StartSaveGroup("EditorUiCompositor");
        settings->SaveBool("Log", _state.logPanel);
        settings->SaveBool("LogAutoscroll", _state.logAutoscroll);
        settings->StartSaveArray("RecentDocuments");
        for (const auto& recent : _recentList)
        {
            settings->SaveString(recent);
        }
        settings->EndSaveArray();
        settings->EndSaveGroup();
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::LoadSettings(Ptr<Settings> settings)
    {
        settings->StartLoadGroup("EditorUiCompositor");
        _state.logPanel = settings->GetBool("Log", true);
        _state.logAutoscroll = settings->GetBool("LogAutoscroll", false);
        const auto recentSize = settings->StartLoadArray("RecentDocuments");
        for (auto i = 0; i < recentSize; i++)
        {
            const auto recentDocument = settings->GetString(i);
            if (!recentDocument.empty())
            {
                _recentList.push_back(recentDocument);
            }
        }
        settings->EndLoadArray();
        settings->EndLoadGroup();
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposeDefaultPanelsLayout()
    {
        STRTLR_CLIENT_LOG_INFO("EditorUiCompositor: composing default UI layout for the first time...");

        auto dockspaceId = ImGui::GetID("EditorDockspace");
        ImGui::DockBuilderRemoveNode(dockspaceId);
        ImGui::DockBuilderAddNode(dockspaceId);
        ImGui::DockBuilderSetNodeSize(dockspaceId, ImGui::GetWindowSize());

        ImGuiID bottom;
        ImGuiID top = ImGui::DockBuilderSplitNode(dockspaceId, ImGuiDir_Up, 0.75f, &dockspaceId, &bottom);

        ImGuiID topRight;
        ImGuiID topLeft;
        ImGui::DockBuilderSplitNode(top, ImGuiDir_Left, 0.5f, &topLeft, &topRight);

        ImGui::DockBuilderDockWindow("###GamePanel", topLeft);
        ImGui::DockBuilderDockWindow("###Properties", topRight);
        ImGui::DockBuilderDockWindow("###Log", bottom);
        ImGui::DockBuilderFinish(dockspaceId);

        ImGui::SaveIniSettingsToDisk(ImGui::GetIO().IniFilename);
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposeMenu()
    {
        if (ImGui::BeginMenuBar())
        {
            ComposeMenuFile();
            ComposeMenuView();

            ImGui::EndMenuBar();
        }
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposeMenuFile()
    {
        if (ImGui::BeginMenu(_translationsDict->Get("File").c_str()))
        {
            ComposeMenuItemNew();
            ComposeMenuItemOpen();
            ComposeMenuItemOpenRecent();
            ComposeMenuItemSave();
            ComposeMenuItemSaveAs();

            ImGui::Separator();

            ComposeMenuItemQuit();
            ComposeMenuItemDemoWindow();

            ImGui::EndMenu();
        }
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposeMenuView()
    {
        if (ImGui::BeginMenu(_translationsDict->Get("View").c_str()))
        {
            ComposeMenuItemLog();
            ComposeMenuItemFullscreen();

            ImGui::EndMenu();
        }
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposeMenuItemNew()
    {
        if (ImGui::MenuItem(_translationsDict->Get("New").c_str(), "Ctrl+N"))
        {
            _popups.newDocument = true;
        }
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposeMenuItemOpen()
    {
        if (ImGui::MenuItem(_translationsDict->Get("Open").c_str(), "Ctrl+O"))
        {
            _popups.openDocument = true;
            _popups.openDocumentFile = "";
        }
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposeMenuItemOpenRecent()
    {
        UiUtils::DisableGuard guard(_recentList.empty());
        if (ImGui::BeginMenu(_translationsDict->Get("Open recent").c_str()))
        {
            for (const auto& recentFile : _recentList)
            {
                const auto recentUnicode = Filesystem::ToU8String(recentFile);
                if (ImGui::MenuItem(recentUnicode.empty() ? recentFile.c_str() : recentUnicode.c_str()))
                {
                    _popups.openDocument = true;
                    _popups.openDocumentFile = recentFile;
                    break;
                }
            }

            if (!_recentList.empty())
            {
                ImGui::Separator();
                if (ImGui::MenuItem(_translationsDict->Get("Clear").c_str()))
                {
                    _recentList.clear();
                }
            }

            ImGui::EndMenu();
        }
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposeMenuItemSave()
    {
        if (ImGui::MenuItem(_translationsDict->Get("Save").c_str(), "Ctrl+S"))
        {
            SaveDocument();
        }
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposeMenuItemSaveAs()
    {
        if (ImGui::MenuItem(_translationsDict->Get("Save as...").c_str(), "Ctrl+Shift+S"))
        {
            SaveAsDocument();
        }
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposeMenuItemQuit()
    {
        if (ImGui::MenuItem(_translationsDict->Get("Quit").c_str(), "Ctrl+Q"))
        {
            _popups.quit = true;
        }
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposeMenuItemDemoWindow()
    {
        ImGui::MenuItem("Demo window", nullptr, &_state.demoWindow);
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposeMenuItemLog()
    {
        ImGui::MenuItem(_translationsDict->Get("Log").c_str(), "Ctrl+L", &_state.logPanel);
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposeMenuItemFullscreen()
    {
        const auto screenMode = _window->GetScreenMode();
        auto isFullscreen = screenMode == Window::WindowedFullscreenMode;
        if (ImGui::MenuItem(_translationsDict->Get("Fullscreen").c_str(), "Alt+Enter", &isFullscreen))
        {
            _window->SetScreenMode(isFullscreen ? Window::WindowedFullscreenMode : Window::WindowedMode);
        }        
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposeGameDocumentPanel()
    {
        const auto document = _gameDocumentManager->GetDocument();
        const auto mainFlags = document->IsDirty() ? ImGuiWindowFlags_UnsavedDocument : ImGuiWindowFlags();
        const auto pathUnicode = Filesystem::ToU8String(document->GetPath());
        auto windowTitle = pathUnicode.empty() ? _translationsDict->Get("Untitled document") : pathUnicode;
        windowTitle.append("###GamePanel");

        if (ImGui::Begin(windowTitle.c_str(), nullptr, mainFlags))
        {
            ComposeGameDocumentPanelGame();
            ComposeGameDocumentPanelObjectsManagement();

            ImGui::SameLine();
            ComposeGameDocumentPanelObjectsTable();

            ImGui::End();
        }
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposeGameDocumentPanelGame()
    {
        ImGui::SeparatorText(_translationsDict->Get("Game document").c_str());

        const auto document = _gameDocumentManager->GetDocument();

        {
            const auto& title = _translationsDict->Get("Name");
            UiUtils::ItemWidthGuard guard(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(title.c_str()).x - ImGui::GetStyle().ItemSpacing.x);
            auto gameName = document->GetGameName();
            const auto oldGameName = gameName;
            if (ImGui::InputText(title.c_str(), &gameName, ImGuiInputTextFlags_EnterReturnsTrue))
            {
                if (oldGameName != gameName)
                {
                    if (gameName.empty())
                    {
                        _popups.warningMessage = true;
                        _popups.warningMessageText = _translationsDict->Get("Game name cannot be empty!");
                    }
                    else
                    {
                        document->SetGameName(gameName);
                    }
                }
            }
        }

        if (ImGui::Button(_translationsDict->Get("Create translations file").c_str()))
        {
            const auto documentPath = document->GetPath();
            std::string filepath;
            if (Filesystem::PathExists(documentPath) && Filesystem::FilePathIsValid(documentPath))
            {
                filepath = Filesystem::ToU8String(documentPath.parent_path().append(document->GetGameName())).append(".txt");
            }
            else
            {
                filepath = Dialogs::SaveFile(_translationsDict->Get("Save translations"), { "Text Files", "*.txt" });
            }

            if (!filepath.empty())
            {
                _localizationManager->CreateTranslations(document, filepath);
            }
        }
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposeGameDocumentPanelObjectsManagement()
    {
        ImGui::SeparatorText(_translationsDict->Get("Objects management").c_str());

        const auto proxy = _gameDocumentManager->GetProxy();

        {
            UiUtils::GroupGuard groupGuard;

            if (ImGui::Button(ICON_FK_PLUS))
            {
                ImGui::OpenPopup("AddObjectPopup");
            }
            UiUtils::SetItemTooltip(_translationsDict->Get("Add object").c_str());

            std::string typeItems[] = {
                _localizationManager->Translate(STRTLR_TR_DOMAIN_ENGINE, ObjectTypeToString(ObjectType::QuestObjectType)),
                _localizationManager->Translate(STRTLR_TR_DOMAIN_ENGINE, ObjectTypeToString(ObjectType::ActionObjectType)),
            };

            if (ImGui::BeginPopup("AddObjectPopup"))
            {
                for (auto typeIndex = 0; typeIndex < 2; typeIndex++)
                {
                    if (ImGui::Selectable(typeItems[typeIndex].c_str()))
                    {
                        proxy->AddObject(ObjectType(typeIndex));
                    }
                }

                ImGui::EndPopup();
            }

            if (ImGui::Button(ICON_FK_EYE))
            {
                ImGui::OpenPopup("ObjectFilterPopup");
            }
            UiUtils::SetItemTooltip(_translationsDict->Get("Visibility filters").c_str());

            if (ImGui::BeginPopup("ObjectFilterPopup"))
            {
                ComposeGameDocumentPanelFilterCheckbox(ObjectType::QuestObjectType, _state.questObjectFilter);
                ComposeGameDocumentPanelFilterCheckbox(ObjectType::ActionObjectType, _state.actionObjectFilter);

                ImGui::EndPopup();
            }
        }
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposeGameDocumentPanelFilterCheckbox(ObjectType objectType, bool& filterState)
    {
        const auto proxy = _gameDocumentManager->GetProxy();

        if (ImGui::Checkbox(_localizationManager->Translate(STRTLR_TR_DOMAIN_ENGINE, ObjectTypeToString(objectType)).c_str(), &filterState))
        {
            if (filterState)
            {
                proxy->UpdateCache();
            }

            proxy->DoFilter(objectType, filterState);
        }
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposeGameDocumentPanelObjectsTable()
    {
        const auto proxy = _gameDocumentManager->GetProxy();

        const auto objectsTableFlags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable
            | ImGuiTableFlags_ScrollY | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_Sortable;

        UiUtils::StyleColorGuard titleColorGuard({
            {ImGuiCol_Header, ImColor(170, 150, 120, 100)},
            {ImGuiCol_HeaderHovered, ImColor(180, 160, 130, 140)},
            {ImGuiCol_HeaderActive, ImColor(180, 160, 130, 180)}
        });

        const auto objectsCount = proxy->GetObjects().size();
        const auto summaryText = LocalizationTranslator::Format(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "total {1} object", "total {1} objects", objectsCount), objectsCount);
        const auto tableOuterSize = ImVec2(0.0f, ImGui::GetContentRegionAvail().y - ImGui::CalcTextSize(summaryText.c_str()).y - ImGui::GetStyle().ItemSpacing.y);
        if (ImGui::BeginTable(_translationsDict->Get("Objects").c_str(), 4, objectsTableFlags, tableOuterSize))
        {
            ImGui::TableSetupColumn(_translationsDict->Get("Actions").c_str(), ImGuiTableColumnFlags_NoResize | ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoSort, 65.0f);
            ImGui::TableSetupColumn(_translationsDict->Get("Type").c_str(), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultSort);
            ImGui::TableSetupColumn(_translationsDict->Get("UUID").c_str(), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultSort);
            ImGui::TableSetupColumn(_translationsDict->Get("Name").c_str(), ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_DefaultSort);
            ImGui::TableSetupScrollFreeze(0, 1);
            ImGui::TableHeadersRow();

            if (auto* sortSpecs = ImGui::TableGetSortSpecs())
            {
                if (sortSpecs->SpecsDirty)
                {
                    const auto tableSortSpec = sortSpecs->Specs;
                    const auto direction = tableSortSpec->SortDirection;
                    proxy->DoSort(direction == ImGuiSortDirection_Ascending, GameDocumentSortFilterProxyView::Sorter::SortValue(tableSortSpec->ColumnIndex - 1));
                    sortSpecs->SpecsDirty = false;
                }
            }

            auto objects = proxy->GetObjects();
            for (auto row = 0; row < objects.size(); row++)
            {
                ImGui::TableNextRow();

                const auto object = objects[row];
                const auto consistent = object->IsConsistent();
                auto selected = proxy->IsSelected(object->GetUuid());

                {
                    UiUtils::IDGuard guard(object->GetUuid());
                    ImGui::TableNextColumn();
                    if (ImGui::Button(ICON_FK_TRASH))
                    {
                        if (object->GetObjectType() == ObjectType::QuestObjectType)
                        {
                            const auto actionObjects = _gameDocumentManager->GetProxy()->GetObjects<ActionObject>();
                            for (const auto actionObject : actionObjects)
                            {
                                if (actionObject->GetTargetUuid() == object->GetUuid())
                                {
                                    actionObject->SetTargetUuid(UUID::InvalidUuid);
                                }
                            }
                        }

                        proxy->RemoveObject(object->GetUuid());
                        objects = proxy->GetObjects();
                    }
                    UiUtils::SetItemTooltip(_translationsDict->Get("Delete object").c_str());

                    if (object->GetObjectType() == ObjectType::ActionObjectType)
                    {
                        const auto actionObject = dynamic_cast<ActionObject*>(object.get());
                        const auto actionHasValidTarget = actionObject && actionObject->GetTargetUuid() != UUID::InvalidUuid && proxy->GetObject(actionObject->GetTargetUuid());
                        ImGui::SameLine();

                        {
                            UiUtils::DisableGuard disableGuard(!actionHasValidTarget);
                            if (ImGui::Button(ICON_FK_SEARCH))
                            {
                                if (actionHasValidTarget)
                                {
                                    proxy->Select(actionObject->GetTargetUuid());
                                }
                            }
                        }
                        UiUtils::SetItemTooltip(_translationsDict->Get("Find object").c_str());
                    }
                }

                {
                    UiUtils::StyleColorGuard guard({ {ImGuiCol_Text, consistent ? ImGui::GetStyleColorVec4(ImGuiCol_Text) : ImVec4(1.0f, 0.5f, 0.5f, 1.0f)}});

                    ImGui::TableNextColumn();
                    ImGui::Selectable(_localizationManager->Translate(STRTLR_TR_DOMAIN_ENGINE, ObjectTypeToString(object->GetObjectType())).c_str(), &selected, ImGuiSelectableFlags_SpanAllColumns);

                    if (ImGui::IsItemClicked(0))
                    {
                        proxy->Select(object->GetUuid());
                    }

                    ImGui::TableNextColumn();
                    ImGui::Text(std::to_string(object->GetUuid()).c_str());

                    ImGui::TableNextColumn();
                    ImGui::Text(object->GetName().c_str());
                }
            }

            ImGui::EndTable();
        }

        ImGui::TextUnformatted(summaryText.c_str());
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposePropertiesPanel()
    {
        auto title = _translationsDict->Get("Properties");
        if (ImGui::Begin(title.append("###Properties").c_str(), nullptr))
        {
            const auto selectedObject = _gameDocumentManager->GetProxy()->GetSelectedObject();

            if (selectedObject)
            {
                const auto selectedObjectType = selectedObject->GetObjectType();
                if (selectedObjectType == ObjectType::QuestObjectType)
                {
                    ComposePropertiesPanelName(selectedObject);
                    ComposePropertiesPanelTexts(selectedObject);
                    ComposePropertiesPanelQuestObject(selectedObject);
                }
                else if (selectedObjectType == ObjectType::ActionObjectType)
                {
                    ComposePropertiesPanelName(selectedObject);
                    ComposePropertiesPanelTexts(selectedObject);
                    ComposePropertiesPanelActionObject(selectedObject);
                }
            }

            ImGui::End();
        }
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposePropertiesPanelName(Ptr<BasicObject> selectedObject)
    {
        assert(selectedObject);

        ImGui::SeparatorText(_translationsDict->Get("Name").c_str());

        const auto uuidString = std::to_string(selectedObject->GetUuid());

        UiUtils::ItemWidthGuard guard(-FLT_MIN);
        auto objectName = selectedObject->GetName();
        const auto oldObjectName = objectName;
        if (ImGui::InputText(std::string("##ObjectName").append(uuidString).c_str(), &objectName, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            if (objectName.empty())
            {
                _popups.warningMessage = true;
                _popups.warningMessageText = _translationsDict->Get("Object name cannot be empty!");
                return;
            }

            if (oldObjectName != objectName && !_gameDocumentManager->GetProxy()->SetObjectName(selectedObject->GetUuid(), objectName))
            {
                _popups.warningMessage = true;
                _popups.warningMessageText = _translationsDict->Get("Object name already exists!");
            }
        }
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposePropertiesPanelTexts(Ptr<BasicObject> selectedObject)
    {
        assert(selectedObject);

        ImGui::SeparatorText(_translationsDict->Get("Source text").c_str());

        const auto uuidString = std::to_string(selectedObject->GetUuid());
        const auto textPanelHeight = ImGui::GetContentRegionAvail().y / 4.0f;
        const auto selectedTextObject = dynamic_cast<TextObject*>(selectedObject.get());
        auto sourceText = selectedTextObject ? selectedTextObject->GetText() : std::string();

        if (ImGui::InputTextMultiline(std::string("##ObjectText").append(uuidString).c_str(), &sourceText, ImVec2(-FLT_MIN, textPanelHeight), ImGuiInputTextFlags_EnterReturnsTrue) && selectedTextObject)
        {
            selectedTextObject->SetText(sourceText);
        }

        ImGui::SeparatorText(_translationsDict->Get("Translation").c_str());

        auto sourceTextTranslation = selectedTextObject ? _localizationManager->Translate(_gameDocumentManager->GetDocument()->GetGameName(), selectedTextObject->GetText()) : std::string();
        ImGui::InputTextMultiline(std::string("##Translation").append(uuidString).c_str(), &sourceTextTranslation, ImVec2(-FLT_MIN, textPanelHeight), ImGuiInputTextFlags_ReadOnly);
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposePropertiesPanelQuestObject(Ptr<BasicObject> selectedObject)
    {
        assert(selectedObject);

        ImGui::SeparatorText(_localizationManager->Translate(STRTLR_TR_DOMAIN_ENGINE, ObjectTypeToString(selectedObject->GetObjectType())).c_str());

        const auto proxy = _gameDocumentManager->GetProxy();
        const auto selectedUuid = selectedObject->GetUuid();

        auto selectedQuestObject = dynamic_cast<QuestObject*>(selectedObject.get());
        const auto allActionObjects = proxy->GetObjects(ObjectType::ActionObjectType);

        const auto entryPointObject = proxy->GetEntryPoint();
        auto isEntryPoint = entryPointObject ? (entryPointObject->GetUuid() == selectedUuid) : false;

        if (ImGui::Checkbox(_translationsDict->Get("Entry point").c_str(), &isEntryPoint))
        {
            proxy->SetEntryPoint(selectedUuid);
        }

        if (_state.selectedActionIndex >= allActionObjects.size())
        {
            _state.selectedActionIndex = 0;
        }

        auto isFinal = selectedQuestObject->IsFinal();
        ImGui::SameLine();
        if (ImGui::Checkbox(_translationsDict->Get("Final").c_str(), &isFinal))
        {
            selectedQuestObject->SetFinal(isFinal);
        }

        {
            UiUtils::DisableGuard guard(allActionObjects.empty());
            if (ImGui::Button(ICON_FK_PLUS))
            {
                selectedQuestObject->AddAction(allActionObjects.at(_state.selectedActionIndex)->GetUuid());
            }
        }
        UiUtils::SetItemTooltip(_translationsDict->Get("Add action to object").c_str());

        ImGui::SameLine();

        {
            const auto title = _translationsDict->Get("Action name");
            UiUtils::ItemWidthGuard guard(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(title.c_str()).x - ImGui::GetStyle().ItemSpacing.x);
            UiUtils::DisableGuard disableGuard(allActionObjects.empty());
            if (ImGui::BeginCombo(title.c_str(), allActionObjects.empty() ? "" : allActionObjects[_state.selectedActionIndex]->GetName().c_str()))
            {
                for (auto actionIndex = 0; actionIndex < allActionObjects.size(); actionIndex++)
                {
                    const auto selected = _state.selectedActionIndex == actionIndex;
                    if (ImGui::Selectable(allActionObjects.at(actionIndex)->GetName().c_str(), selected))
                    {
                        _state.selectedActionIndex = actionIndex;
                    }

                    if (selected)
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                }

                ImGui::EndCombo();
            }
        }

        const auto questObjectActions = selectedQuestObject->GetActions();
        if (_state.selectedChildActionIndex >= questObjectActions.size())
        {
            _state.selectedChildActionIndex = 0;
        }

        ImGui::SeparatorText(_translationsDict->Get("Actions").c_str());

        const auto actionsTableFlags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable 
            | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_ScrollY;

        UiUtils::StyleColorGuard titleColorGuard({
            {ImGuiCol_Header, ImColor(170, 150, 120, 100)},
            {ImGuiCol_HeaderHovered, ImColor(180, 160, 130, 140)},
            {ImGuiCol_HeaderActive, ImColor(180, 160, 130, 180)}
        });

        {
            UiUtils::GroupGuard groupGuard;

            const auto selectedActionObject = questObjectActions.empty() ? UUID::InvalidUuid : questObjectActions[_state.selectedChildActionIndex];

            {
                UiUtils::DisableGuard disableGuard(questObjectActions.empty() || _state.selectedChildActionIndex == 0);
                if (ImGui::Button(ICON_FK_ARROW_UP))
                {
                    selectedQuestObject->MoveActionUp(selectedActionObject);
                    _state.selectedChildActionIndex--;
                }
            }
            UiUtils::SetItemTooltip(_translationsDict->Get("Move action up").c_str());

            {
                UiUtils::DisableGuard disableGuard(questObjectActions.empty() || _state.selectedChildActionIndex >= (questObjectActions.size() - 1));
                if (ImGui::Button(ICON_FK_ARROW_DOWN))
                {
                    selectedQuestObject->MoveActionDown(selectedActionObject);
                    _state.selectedChildActionIndex++;
                }
            }
            UiUtils::SetItemTooltip(_translationsDict->Get("Move action down").c_str());
        }

        ImGui::SameLine();
        if (ImGui::BeginTable(_translationsDict->Get("Objects's actions").c_str(), 3, actionsTableFlags))
        {
            ImGui::TableSetupColumn(_translationsDict->Get("Actions").c_str(), ImGuiTableColumnFlags_NoResize | ImGuiTableColumnFlags_WidthFixed, 65.0f);
            ImGui::TableSetupColumn(_translationsDict->Get("Name").c_str(), ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn(_translationsDict->Get("Text").c_str(), ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupScrollFreeze(0, 1);
            ImGui::TableHeadersRow();

            for (auto row = 0; row < questObjectActions.size(); row++)
            {
                const auto object = proxy->GetObject(questObjectActions[row]);
                if (!object)
                {
                    continue;
                }

                const auto actionObject = dynamic_cast<ActionObject*>(object.get());
                if (!actionObject)
                {
                    continue;
                }

                auto selected = _state.selectedChildActionIndex == row;

                ImGui::TableNextRow();

                {
                    UiUtils::IDGuard guard(actionObject->GetUuid());
                    ImGui::TableNextColumn();
                    if (ImGui::Button(ICON_FK_TRASH))
                    {
                        selectedQuestObject->RemoveAction(actionObject->GetUuid());
                        continue;
                    }
                    UiUtils::SetItemTooltip(_translationsDict->Get("Remove action from object").c_str());

                    ImGui::SameLine();
                    if (ImGui::Button(ICON_FK_SEARCH))
                    {
                        proxy->Select(actionObject->GetUuid());
                        break;
                    }
                    UiUtils::SetItemTooltip(_translationsDict->Get("Find action object").c_str());
                }

                UiUtils::StyleColorGuard guard({ {ImGuiCol_Text, actionObject->IsConsistent() ? ImGui::GetStyleColorVec4(ImGuiCol_Text) : ImVec4(1.0f, 0.5f, 0.5f, 1.0f)}});

                ImGui::TableNextColumn();
                ImGui::Selectable(object->GetName().c_str(), &selected, ImGuiSelectableFlags_SpanAllColumns);
                if (ImGui::IsItemClicked(0))
                {
                    _state.selectedChildActionIndex = row;
                }

                ImGui::TableNextColumn();
                ImGui::Text(_localizationManager->Translate(_gameDocumentManager->GetDocument()->GetGameName(), actionObject->GetText()).c_str());
            }

            ImGui::EndTable();
        }
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposePropertiesPanelActionObject(Ptr<BasicObject> selectedObject)
    {
        assert(selectedObject);

        ImGui::SeparatorText(_localizationManager->Translate(STRTLR_TR_DOMAIN_ENGINE, ObjectTypeToString(selectedObject->GetObjectType())).c_str());

        const auto proxy = _gameDocumentManager->GetProxy();
        const auto selectedActionObject = dynamic_cast<ActionObject*>(selectedObject.get());
        const auto allQuestObjects = proxy->GetObjects(ObjectType::QuestObjectType);

        if (_state.selectedQuestIndex >= allQuestObjects.size())
        {
            _state.selectedQuestIndex = 0;
        }

        {
            UiUtils::DisableGuard guard(selectedActionObject->GetTargetUuid() == UUID::InvalidUuid);
            if (ImGui::Button(ICON_FK_CIRCLE_O))
            {
                selectedActionObject->SetTargetUuid(Storyteller::UUID::InvalidUuid);
            }
        }
        UiUtils::SetItemTooltip(_translationsDict->Get("Clear target").c_str());

        ImGui::SameLine();
        {
            UiUtils::DisableGuard guard(allQuestObjects.empty());
            if (ImGui::Button(ICON_FK_BULLSEYE))
            {
                selectedActionObject->SetTargetUuid(allQuestObjects.at(_state.selectedQuestIndex)->GetUuid());
            }
        }
        UiUtils::SetItemTooltip(_translationsDict->Get("Set target").c_str());

        ImGui::SameLine();
        {
            const auto title = _translationsDict->Get("Quest object name");
            UiUtils::ItemWidthGuard guard(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(title.c_str()).x - ImGui::GetStyle().ItemSpacing.x);
            UiUtils::DisableGuard disableGuard(allQuestObjects.empty());
            if (ImGui::BeginCombo(title.c_str(), allQuestObjects.empty() ? "" : allQuestObjects[_state.selectedQuestIndex]->GetName().c_str()))
            {
                for (auto questIndex = 0; questIndex < allQuestObjects.size(); questIndex++)
                {
                    const auto selected = _state.selectedQuestIndex == questIndex;
                    if (ImGui::Selectable(allQuestObjects.at(questIndex)->GetName().c_str(), selected))
                    {
                        _state.selectedQuestIndex = questIndex;
                    }

                    if (selected)
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                }
                ImGui::EndCombo();
            }
        }

        ImGui::Text(_translationsDict->Get("Current target name: ").c_str());
        ImGui::SameLine();
        const auto targetObject = proxy->GetObject(selectedActionObject->GetTargetUuid());
        ImGui::Text(targetObject ? std::string("[").append(targetObject->GetName()).append("]").c_str() : _translationsDict->Get("Not set or does not exist").c_str());


        ImGui::SameLine();
        {
            UiUtils::DisableGuard guard(!(targetObject.get() && targetObject->GetUuid() != UUID::InvalidUuid));
            if (ImGui::Button(ICON_FK_SEARCH))
            {
                proxy->Select(targetObject->GetUuid());
            }
        }
        UiUtils::SetItemTooltip(_translationsDict->Get("Find object").c_str());
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposeLogPanel()
    {
        auto title = _translationsDict->Get("Log");
        ImGui::Begin(title.append("###Log").c_str(), nullptr);

        auto singleScrollToEnd = false;

        {
            UiUtils::GroupGuard groupGuard;

            if (ImGui::Button(ICON_FK_ANGLE_DOWN))
            {
                singleScrollToEnd = true;
            }
            UiUtils::SetItemTooltip(_translationsDict->Get("Scroll to end").c_str());

            {
                UiUtils::StyleColorGuard colorGuard({ {ImGuiCol_Border, _state.logAutoscroll ? ImVec4(1, 1, 1, 1) : ImGui::GetStyleColorVec4(ImGuiCol_Border)} });
                if (ImGui::Button(ICON_FK_ANGLE_DOUBLE_DOWN))
                {
                    _state.logAutoscroll = !_state.logAutoscroll;
                }
            }
            UiUtils::SetItemTooltip(_translationsDict->Get("Autoscroll to end").c_str());
        }

        ImGui::SameLine();

        {
            UiUtils::StyleColorGuard colorGuard({ {ImGuiCol_FrameBg, ImColor(0, 0, 0, 0)} });
            auto logDataStr = std::string(Log::StringLogOutput());
            const char* logViewName = "##LogView";
            ImGui::InputTextMultiline(logViewName, &logDataStr, ImVec2(ImGui::GetContentRegionAvail().x, -FLT_MIN), ImGuiInputTextFlags_ReadOnly);

            if (_state.logAutoscroll || singleScrollToEnd)
            {
                const auto& imguiContext = *GImGui;
                const char* windowName = nullptr;
                ImFormatStringToTempBuffer(&windowName, nullptr, "%s/%s_%08X", imguiContext.CurrentWindow->Name, logViewName, ImGui::GetID(logViewName));
                auto* logWindow = ImGui::FindWindowByName(windowName);
                if (logWindow)
                {
                    ImGui::SetScrollY(logWindow, logWindow->ScrollMax.y);
                }
            }
        }

        ImGui::End();
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposePopups()
    {
        if (_popups.newDocument)
        {
            PopupNewDocument();
        }
        if (_popups.quit)
        {
            PopupQuit();
        }
        if (_popups.warningMessage)
        {
            PopupWarningMessage();
        }
        if (_popups.openDocument)
        {
            PopupOpenDocument();
        }
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::PopupNewDocument()
    {
        if (_gameDocumentManager->GetDocument()->IsDirty())
        {
            const auto& title = _translationsDict->Get("New document");
            ImGui::OpenPopup(title.c_str());
            const auto center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

            if (ImGui::BeginPopupModal(title.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text(_translationsDict->Get("You have unsaved changes, create new document anyway?").c_str());
                ImGui::Separator();

                if (ImGui::Button(_translationsDict->Get("Yes").c_str(), ImVec2(ImGui::GetContentRegionAvail().x / 2, 0)))
                {
                    _gameDocumentManager->NewDocument();
                    _popups.newDocument = false;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SetItemDefaultFocus();

                ImGui::SameLine();
                if (ImGui::Button(_translationsDict->Get("No").c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0)))
                {
                    _popups.newDocument = false;
                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }
        }
        else
        {
            _gameDocumentManager->NewDocument();
            _popups.newDocument = false;
        }
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::PopupQuit()
    {
        if (_gameDocumentManager->GetDocument()->IsDirty())
        {
            const auto& title = _translationsDict->Get("Quit");

            ImGui::OpenPopup(title.c_str());
            const auto center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

            if (ImGui::BeginPopupModal(title.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text(_translationsDict->Get("You have unsaved changes, quit anyway?").c_str());
                ImGui::Separator();

                if (ImGui::Button(_translationsDict->Get("Yes").c_str(), ImVec2(ImGui::GetContentRegionAvail().x / 2, 0)))
                {
                    _window->SetShouldClose(true);
                    _popups.quit = false;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SetItemDefaultFocus();

                ImGui::SameLine();
                if (ImGui::Button(_translationsDict->Get("No").c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0)))
                {
                    _window->SetShouldClose(false);
                    _popups.quit = false;
                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }
        }
        else
        {
            _window->SetShouldClose(true);
            _popups.quit = false;
        }
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::PopupWarningMessage()
    {
        const auto& title = _translationsDict->Get("Warning");
        ImGui::OpenPopup(title.c_str());
        const auto center = ImGui::GetMainViewport()->GetCenter();
        ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

        if (ImGui::BeginPopupModal(title.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
        {
            ImGui::Text(_popups.warningMessageText.c_str());
            ImGui::Separator();

            if (ImGui::Button(_translationsDict->Get("Ok").c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0)))
            {
                _popups.warningMessage = false;
                ImGui::CloseCurrentPopup();
            }
            ImGui::SetItemDefaultFocus();

            ImGui::EndPopup();
        }
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::PopupOpenDocument()
    {
        if (_gameDocumentManager->GetDocument()->IsDirty())
        {
            const auto& title = _translationsDict->Get("Open");

            ImGui::OpenPopup(title.c_str());
            const auto center = ImGui::GetMainViewport()->GetCenter();
            ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

            if (ImGui::BeginPopupModal(title.c_str(), nullptr, ImGuiWindowFlags_AlwaysAutoResize))
            {
                ImGui::Text(_translationsDict->Get("You have unsaved changes, open other document anyway?").c_str());
                ImGui::Separator();

                if (ImGui::Button(_translationsDict->Get("Yes").c_str(), ImVec2(ImGui::GetContentRegionAvail().x / 2, 0)))
                {
                    if (_popups.openDocumentFile.empty())
                    {
                        const auto filepath = Dialogs::OpenFile(_translationsDict->Get("Open document"), {"JSON Files", "*.json"});
                        if (!filepath.empty())
                        {
                            OpenDocument(filepath);
                        }
                    }
                    else
                    {
                        OpenDocument(_popups.openDocumentFile);
                    }

                    _popups.openDocument = false;
                    ImGui::CloseCurrentPopup();
                }
                ImGui::SetItemDefaultFocus();

                ImGui::SameLine();
                if (ImGui::Button(_translationsDict->Get("No").c_str(), ImVec2(ImGui::GetContentRegionAvail().x, 0)))
                {
                    _popups.openDocument = false;
                    ImGui::CloseCurrentPopup();
                }

                ImGui::EndPopup();
            }
        }
        else
        {
            if (_popups.openDocumentFile.empty())
            {
                const auto filepath = Dialogs::OpenFile(_translationsDict->Get("Open document"), { "JSON Files", "*.json" });
                if (!filepath.empty())
                {
                    OpenDocument(filepath);
                }
            }
            else
            {
                OpenDocument(_popups.openDocumentFile);
            }

            _popups.openDocument = false;
        }
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::SaveDocument()
    {
        if (!_gameDocumentManager->GetDocument()->GetPathString().empty())
        {
            _gameDocumentManager->Save();
        }
        else
        {
            SaveAsDocument();
        }
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::SaveAsDocument()
    {
        const auto filepath = Dialogs::SaveFile(_translationsDict->Get("Save document"), { "JSON Files", "*.json" });
        if (!filepath.empty())
        {
            _gameDocumentManager->Save(filepath);
        }
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::OpenDocument(const std::string& filename)
    {
        const auto success = _gameDocumentManager->OpenDocument(filename);
        _recentList.remove(filename);

        if (success)
        {
            _recentList.push_front(filename);
        }
        else
        {
            _popups.warningMessage = true;
            _popups.warningMessageText = _translationsDict->Get("The selected file is missing or damaged", "Popup message");
        }
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::SwitchLogWindowVisibility()
    {
        _state.logPanel = !_state.logPanel;
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::SwitchFullscreen()
    {
        const auto screenMode = _window->GetScreenMode();
        auto isFullscreen = screenMode == Window::WindowedFullscreenMode;

        _window->SetScreenMode(!isFullscreen ? Window::WindowedFullscreenMode : Window::WindowedMode);
    }
    //--------------------------------------------------------------------------
}