#include "editor_ui_compositor.h"
#include "ui_utils.h"
#include "icons_font.h"
#include "Storyteller/log.h"
#include "Storyteller/dialogs.h"
#include "Storyteller/filesystem_utils.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <misc/cpp/imgui_stdlib.h>

namespace Storyteller
{
    EditorUiCompositor::EditorUiCompositor(Ptr<Window> window, Ptr<LocalizationManager> localizationManager)
        : _window(window)
        , _localizationManager(localizationManager)
        , _gameDocumentManager(new GameDocumentManager())
    {}
    //--------------------------------------------------------------------------

    void EditorUiCompositor::Compose()
    {
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
    }
    //--------------------------------------------------------------------------

    bool EditorUiCompositor::ReadyToClose() const
    {
        if (!_gameDocumentManager->GetDocument()->IsDirty())
        {
            return true;
        }

        const auto sureQuit = Dialogs::Message(_localizationManager->Translate("StorytellerEditor", "You have unsaved changes, quit anyway?").c_str(),
            _localizationManager->Translate("StorytellerEditor", "Quit").c_str(), _window);

        return sureQuit;
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

        if (keyCode == Key::Q && mods == Mode::Ctrl)
        {
            Exit();
        }
        else if (keyCode == Key::N && mods == Mode::Ctrl)
        {
            NewDocument();
        }
        else if (keyCode == Key::O && mods == Mode::Ctrl)
        {
            OpenDocument();
        }
        else if (keyCode == Key::S && mods == Mode::Ctrl)
        {
            SaveDocument();
        }
        else if (keyCode == Key::S && mods == (Mode::Ctrl | Mode::Shift))
        {
            SaveAsDocument();
        }
        else if (keyCode == Key::L && mods == Mode::Ctrl)
        {
            SwitchLogWindowVisibility();
        }
        else if (keyCode == Key::Enter && mods == Mode::Alt)
        {
            SwitchFullscreen();
        }

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
        if (ImGui::BeginMenu(_localizationManager->Translate("StorytellerEditor", "File").c_str()))
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
        if (ImGui::BeginMenu(_localizationManager->Translate("StorytellerEditor", "View").c_str()))
        {
            ComposeMenuItemLog();
            ComposeMenuItemFullscreen();

            ImGui::EndMenu();
        }
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposeMenuItemNew()
    {
        if (ImGui::MenuItem(_localizationManager->Translate("StorytellerEditor", "New").c_str(), "Ctrl+N"))
        {
            NewDocument();
        }
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposeMenuItemOpen()
    {
        if (ImGui::MenuItem(_localizationManager->Translate("StorytellerEditor", "Open").c_str(), "Ctrl+O"))
        {
            OpenDocument();
        }
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposeMenuItemOpenRecent()
    {
        UiUtils::DisableGuard guard(_recentList.empty());
        if (ImGui::BeginMenu(_localizationManager->Translate("StorytellerEditor", "Open recent").c_str()))
        {
            auto recentDirty = false;
            for (const auto recentCopy : _recentList)
            {
                const auto recentUnicode = Filesystem::PathUnicode(recentCopy);
                if (ImGui::MenuItem(recentUnicode.empty() ? recentCopy.c_str() : recentUnicode.c_str()))
                {
                    if (_gameDocumentManager->GetDocument()->IsDirty())
                    {
                        const auto sureOpen = Dialogs::Message(_localizationManager->Translate("StorytellerEditor", "You have unsaved changes, open other document anyway?").c_str(),
                            _localizationManager->Translate("StorytellerEditor", "Open").c_str(), _window);

                        if (sureOpen)
                        {
                            const auto success = _gameDocumentManager->OpenDocument(recentCopy);
                            if (!success)
                            {
                                Dialogs::Message(_localizationManager->Translate("StorytellerEditor", "The selected file is missing or damaged").c_str(),
                                    _localizationManager->Translate("StorytellerEditor", "Warning").c_str(), _window, Dialogs::OkButtons);
                                recentDirty = true;
                            }
                        }
                    }
                    else
                    {
                        const auto success = _gameDocumentManager->OpenDocument(recentCopy);
                        if (!success)
                        {
                            Dialogs::Message(_localizationManager->Translate("StorytellerEditor", "The selected file is missing or damaged").c_str(),
                                _localizationManager->Translate("StorytellerEditor", "Warning").c_str(), _window, Dialogs::OkButtons);
                            recentDirty = true;
                        }
                    }

                    _recentList.remove(recentCopy);
                    if (!recentDirty)
                    {
                        _recentList.push_front(recentCopy);
                    }

                    break;
                }
            }

            if (!_recentList.empty())
            {
                ImGui::Separator();
                if (ImGui::MenuItem(_localizationManager->Translate("StorytellerEditor", "Clear").c_str()))
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
        if (ImGui::MenuItem(_localizationManager->Translate("StorytellerEditor", "Save").c_str(), "Ctrl+S"))
        {
            SaveDocument();
        }
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposeMenuItemSaveAs()
    {
        if (ImGui::MenuItem(_localizationManager->Translate("StorytellerEditor", "Save as...").c_str(), "Ctrl+Shift+S"))
        {
            SaveAsDocument();
        }
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposeMenuItemQuit()
    {
        if (ImGui::MenuItem(_localizationManager->Translate("StorytellerEditor", "Quit").c_str(), "Ctrl+Q"))
        {
            Exit();
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
        ImGui::MenuItem(_localizationManager->Translate("StorytellerEditor", "Log").c_str(), "Ctrl+L", &_state.logPanel);
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposeMenuItemFullscreen()
    {
        const auto screenMode = _window->GetScreenMode();
        auto isFullscreen = screenMode == Window::WindowedFullscreenMode;
        if (ImGui::MenuItem(_localizationManager->Translate("StorytellerEditor", "Fullscreen").c_str(), "Alt+Enter", &isFullscreen))
        {
            _window->SetScreenMode(isFullscreen ? Window::WindowedFullscreenMode : Window::WindowedMode);
        }        
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposeGameDocumentPanel()
    {
        const auto document = _gameDocumentManager->GetDocument();
        const auto mainFlags = document->IsDirty() ? ImGuiWindowFlags_UnsavedDocument : ImGuiWindowFlags();
        const auto pathUnicode = Filesystem::PathUnicode(document->GetPath());
        auto windowTitle = pathUnicode.empty() ? _localizationManager->Translate("StorytellerEditor", "Untitled document") : pathUnicode;
        windowTitle.append("###").append("GamePanel");

        if (ImGui::Begin(windowTitle.c_str(), nullptr, mainFlags))
        {
            ComposeGameDocumentPanelGame();
            ComposeGameDocumentPanelObjectsManagement();
            ComposeGameDocumentPanelFilters();
            ComposeGameDocumentPanelObjectsTable();

            ImGui::End();
        }
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposeGameDocumentPanelGame()
    {
        ImGui::SeparatorText(_localizationManager->Translate("StorytellerEditor", "Game document").c_str());

        const auto document = _gameDocumentManager->GetDocument();

        {
            const auto title = _localizationManager->Translate("StorytellerEditor", "Name");
            UiUtils::ItemWidthGuard guard(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(title.c_str()).x);
            auto gameName = document->GetGameName();
            if (ImGui::InputText(title.c_str(), &gameName, ImGuiInputTextFlags_EnterReturnsTrue))
            {
                document->SetGameName(gameName);
            }
        }

        if (ImGui::Button(_localizationManager->Translate("StorytellerEditor", "Create translations file...").c_str()))
        {
            const auto filepath = Dialogs::SaveFile("Text Files (*.txt)\0*.txt\0", _window);
            _localizationManager->CreateTranslations(document, filepath);
        }
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposeGameDocumentPanelObjectsManagement()
    {
        ImGui::SeparatorText(_localizationManager->Translate("StorytellerEditor", "Objects management").c_str());

        const auto proxy = _gameDocumentManager->GetProxy();

        if (ImGui::Button(ICON_FK_PLUS))
        {
            proxy->AddObject(ObjectType(_state.selectedTypeIndex));
        }
        UiUtils::SetItemTooltip(_localizationManager->Translate("StorytellerEditor", "Add object").c_str());

        ImGui::SameLine();

        std::string typeItems[] = {
            _localizationManager->Translate("Storyteller", ObjectTypeToString(ObjectType::QuestObjectType)),
            _localizationManager->Translate("Storyteller", ObjectTypeToString(ObjectType::ActionObjectType)),
        };

        {
            const auto title = _localizationManager->Translate("StorytellerEditor", "Type");
            UiUtils::ItemWidthGuard guard(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(title.c_str()).x);
            if (ImGui::BeginCombo(title.c_str(), typeItems[_state.selectedTypeIndex].c_str()))
            {
                for (auto typeIndex = 0; typeIndex < 2; typeIndex++)
                {
                    const auto selected = _state.selectedTypeIndex == typeIndex;
                    if (ImGui::Selectable(typeItems[typeIndex].c_str(), selected))
                    {
                        _state.selectedTypeIndex = typeIndex;
                    }

                    if (selected)
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                }

                ImGui::EndCombo();
            }
        }
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposeGameDocumentPanelFilters()
    {
        ComposeGameDocumentPanelFilterCheckbox(ObjectType::QuestObjectType, _state.questObjectFilter);
        ImGui::SameLine();
        ComposeGameDocumentPanelFilterCheckbox(ObjectType::ActionObjectType, _state.actionObjectFilter);
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposeGameDocumentPanelFilterCheckbox(ObjectType objectType, bool& filterState)
    {
        const auto proxy = _gameDocumentManager->GetProxy();

        if (ImGui::Checkbox(_localizationManager->Translate("Storyteller", ObjectTypeToString(objectType)).c_str(), &filterState))
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

        if (ImGui::BeginTable(_localizationManager->Translate("StorytellerEditor", "Objects").c_str(), 4, objectsTableFlags))
        {
            ImGui::TableSetupColumn(_localizationManager->Translate("StorytellerEditor", "Actions").c_str(), ImGuiTableColumnFlags_NoResize | ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoSort);
            ImGui::TableSetupColumn(_localizationManager->Translate("StorytellerEditor", "Type").c_str(), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultSort);
            ImGui::TableSetupColumn(_localizationManager->Translate("StorytellerEditor", "UUID").c_str(), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultSort);
            ImGui::TableSetupColumn(_localizationManager->Translate("StorytellerEditor", "Name").c_str(), ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_DefaultSort);
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
                            const auto actionObjects = _gameDocumentManager->GetProxy()->GetObjects<ActionObject>(false);
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
                    UiUtils::SetItemTooltip(_localizationManager->Translate("StorytellerEditor", "Delete object").c_str());

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
                        UiUtils::SetItemTooltip(_localizationManager->Translate("StorytellerEditor", "Find object").c_str());
                    }
                }

                {
                    UiUtils::StyleColorGuard guard({ {ImGuiCol_Text, consistent ? ImVec4(1.0f, 1.0f, 1.0f, 1.0f) : ImVec4(1.0f, 0.5f, 0.5f, 1.0f)} });

                    ImGui::TableNextColumn();
                    ImGui::Selectable(_localizationManager->Translate("Storyteller", ObjectTypeToString(object->GetObjectType())).c_str(), &selected, ImGuiSelectableFlags_SpanAllColumns);

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
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposePropertiesPanel()
    {
        if (ImGui::Begin(_localizationManager->Translate("StorytellerEditor", "Properties").c_str(), nullptr))
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

        ImGui::SeparatorText(_localizationManager->Translate("StorytellerEditor", "Name").c_str());

        const auto uuidString = std::to_string(selectedObject->GetUuid());

        UiUtils::ItemWidthGuard guard(-FLT_MIN);
        auto objectName = selectedObject->GetName();
        const auto oldObjectName = objectName;
        if (ImGui::InputText(std::string("##ObjectName").append(uuidString).c_str(), &objectName, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            if (oldObjectName != objectName && !_gameDocumentManager->GetProxy()->SetObjectName(selectedObject->GetUuid(), objectName))
            {
                Dialogs::Message(_localizationManager->Translate("StorytellerEditor", "Object name already exists").c_str(), 
                    _localizationManager->Translate("StorytellerEditor", "Warning").c_str(), _window, Dialogs::OkButtons);
            }
        }
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposePropertiesPanelTexts(Ptr<BasicObject> selectedObject)
    {
        assert(selectedObject);

        ImGui::SeparatorText(_localizationManager->Translate("StorytellerEditor", "Source text").c_str());

        const auto uuidString = std::to_string(selectedObject->GetUuid());
        const auto textPanelHeight = ImGui::GetContentRegionAvail().y / 4.0f;
        const auto selectedTextObject = dynamic_cast<TextObject*>(selectedObject.get());
        auto sourceText = selectedTextObject ? selectedTextObject->GetText() : std::string();

        if (ImGui::InputTextMultiline(std::string("##ObjectText").append(uuidString).c_str(), &sourceText, ImVec2(-FLT_MIN, textPanelHeight), ImGuiInputTextFlags_EnterReturnsTrue) && selectedTextObject)
        {
            selectedTextObject->SetText(sourceText);
        }

        ImGui::SeparatorText(_localizationManager->Translate("StorytellerEditor", "Translation").c_str());

        auto sourceTextTranslation = selectedTextObject ? _localizationManager->Translate(_gameDocumentManager->GetDocument()->GetGameName(), sourceText, true) : std::string();
        ImGui::InputTextMultiline(std::string("##Translation").append(uuidString).c_str(), &sourceTextTranslation, ImVec2(-FLT_MIN, textPanelHeight), ImGuiInputTextFlags_ReadOnly);
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposePropertiesPanelQuestObject(Ptr<BasicObject> selectedObject)
    {
        assert(selectedObject);

        ImGui::SeparatorText(_localizationManager->Translate("Storyteller", ObjectTypeToString(selectedObject->GetObjectType())).c_str());

        const auto proxy = _gameDocumentManager->GetProxy();
        const auto selectedUuid = selectedObject->GetUuid();

        auto selectedQuestObject = dynamic_cast<QuestObject*>(selectedObject.get());
        const auto namedActionObjects = proxy->GetObjects(ObjectType::ActionObjectType, true);

        const auto entryPointObject = proxy->GetEntryPoint();
        auto isEntryPoint = entryPointObject ? (entryPointObject->GetUuid() == selectedUuid) : false;

        if (ImGui::Checkbox(_localizationManager->Translate("StorytellerEditor", "Entry point").c_str(), &isEntryPoint))
        {
            proxy->SetEntryPoint(selectedUuid);
        }

        if (_state.selectedActionIndex >= namedActionObjects.size())
        {
            _state.selectedActionIndex = 0;
        }

        auto isFinal = selectedQuestObject->IsFinal();
        ImGui::SameLine();
        if (ImGui::Checkbox(_localizationManager->Translate("StorytellerEditor", "Final").c_str(), &isFinal))
        {
            selectedQuestObject->SetFinal(isFinal);
        }

        {
            UiUtils::DisableGuard guard(namedActionObjects.empty());
            if (ImGui::Button(ICON_FK_PLUS))
            {
                selectedQuestObject->AddAction(namedActionObjects.at(_state.selectedActionIndex)->GetUuid());
            }
        }
        UiUtils::SetItemTooltip(_localizationManager->Translate("StorytellerEditor", "Add action to object").c_str());

        ImGui::SameLine();

        {
            const auto title = _localizationManager->Translate("StorytellerEditor", "Action name");
            UiUtils::ItemWidthGuard guard(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(title.c_str()).x);
            UiUtils::DisableGuard disableGuard(namedActionObjects.empty());
            if (ImGui::BeginCombo(title.c_str(), namedActionObjects.empty() ? "" : namedActionObjects[_state.selectedActionIndex]->GetName().c_str()))
            {
                for (auto actionIndex = 0; actionIndex < namedActionObjects.size(); actionIndex++)
                {
                    const auto selected = _state.selectedActionIndex == actionIndex;
                    if (ImGui::Selectable(namedActionObjects.at(actionIndex)->GetName().c_str(), selected))
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

        ImGui::SeparatorText(_localizationManager->Translate("Storyteller", "Actions").c_str());

        const auto actionsTableFlags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable 
            | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_ScrollY;
        if (ImGui::BeginTable(_localizationManager->Translate("StorytellerEditor", "Objects's actions").c_str(), 4, actionsTableFlags))
        {
            ImGui::TableSetupColumn(_localizationManager->Translate("StorytellerEditor", "Actions").c_str(), ImGuiTableColumnFlags_NoResize | ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn(_localizationManager->Translate("StorytellerEditor", "UUID").c_str(), ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn(_localizationManager->Translate("StorytellerEditor", "Name").c_str(), ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn(_localizationManager->Translate("StorytellerEditor", "Text").c_str(), ImGuiTableColumnFlags_WidthStretch);
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
                    UiUtils::SetItemTooltip(_localizationManager->Translate("StorytellerEditor", "Remove action from object").c_str());

                    ImGui::SameLine();
                    if (ImGui::Button(ICON_FK_SEARCH))
                    {
                        proxy->Select(actionObject->GetUuid());
                        break;
                    }
                    UiUtils::SetItemTooltip(_localizationManager->Translate("StorytellerEditor", "Find action object").c_str());
                }

                UiUtils::StyleColorGuard guard({ {ImGuiCol_Text, actionObject->IsConsistent() ? ImVec4(1.0f, 1.0f, 1.0f, 1.0f) : ImVec4(1.0f, 0.5f, 0.5f, 1.0f)}});

                ImGui::TableNextColumn();
                ImGui::Selectable(std::to_string(actionObject->GetUuid()).c_str(), &selected, ImGuiSelectableFlags_SpanAllColumns);
                if (ImGui::IsItemClicked(0))
                {
                    _state.selectedChildActionIndex = row;
                }

                ImGui::TableNextColumn();
                ImGui::Text(object->GetName().c_str());

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

        ImGui::SeparatorText(_localizationManager->Translate("Storyteller", ObjectTypeToString(selectedObject->GetObjectType())).c_str());

        const auto proxy = _gameDocumentManager->GetProxy();
        const auto selectedActionObject = dynamic_cast<ActionObject*>(selectedObject.get());
        const auto allQuestObjects = proxy->GetObjects(ObjectType::QuestObjectType, true);

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
        UiUtils::SetItemTooltip(_localizationManager->Translate("StorytellerEditor", "Clear target").c_str());

        ImGui::SameLine();
        {
            UiUtils::DisableGuard guard(allQuestObjects.empty());
            if (ImGui::Button(ICON_FK_BULLSEYE))
            {
                selectedActionObject->SetTargetUuid(allQuestObjects.at(_state.selectedQuestIndex)->GetUuid());
            }
        }
        UiUtils::SetItemTooltip(_localizationManager->Translate("StorytellerEditor", "Set target").c_str());

        ImGui::SameLine();
        {
            const auto title = _localizationManager->Translate("StorytellerEditor", "Quest object name");
            UiUtils::ItemWidthGuard guard(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(title.c_str()).x);
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

        ImGui::Text(_localizationManager->Translate("StorytellerEditor", "Current target name: ").c_str());
        ImGui::SameLine();
        const auto targetObject = proxy->GetObject(selectedActionObject->GetTargetUuid());
        ImGui::Text(targetObject ? std::string("[").append(targetObject->GetName()).append("]").c_str() : _localizationManager->Translate("StorytellerEditor", "Not set or does not exist").c_str());


        ImGui::SameLine();
        {
            UiUtils::DisableGuard guard(!(targetObject.get() && targetObject->GetUuid() != UUID::InvalidUuid));
            if (ImGui::Button(ICON_FK_SEARCH))
            {
                proxy->Select(targetObject->GetUuid());
            }
        }
        UiUtils::SetItemTooltip(_localizationManager->Translate("StorytellerEditor", "Find object").c_str());
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposeLogPanel()
    {
        ImGui::Begin(_localizationManager->Translate("StorytellerEditor", "Log").c_str(), nullptr);

        {
            UiUtils::GroupGuard groupGuard;

            {
                UiUtils::StyleColorGuard colorGuard({ {ImGuiCol_Border, _state.logAutoscroll ? ImVec4(1, 1, 1, 1) : ImGui::GetStyleColorVec4(ImGuiCol_Border)} });
                if (ImGui::Button(ICON_FK_ARROW_DOWN))
                {
                    _state.logAutoscroll = !_state.logAutoscroll;
                }
            }
            UiUtils::SetItemTooltip(_localizationManager->Translate("StorytellerEditor", "Autoscroll to end").c_str());
        }

        ImGui::SameLine();

        {
            UiUtils::StyleColorGuard colorGuard({ {ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0)} });
            auto logDataStr = std::string(Log::StringLogOutput());
            const char* logViewName = "##LogView";
            ImGui::InputTextMultiline(logViewName, &logDataStr, ImVec2(ImGui::GetContentRegionAvail().x, -FLT_MIN), ImGuiInputTextFlags_ReadOnly);

            if (_state.logAutoscroll)
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

    void EditorUiCompositor::NewDocument()
    {
        if (_gameDocumentManager->GetDocument()->IsDirty())
        {
            const auto sureNew = Dialogs::Message(_localizationManager->Translate("StorytellerEditor", "You have unsaved changes, create new document anyway?").c_str(),
                _localizationManager->Translate("StorytellerEditor", "New").c_str(), _window);

            if (sureNew)
            {
                _gameDocumentManager->NewDocument();
            }
        }
        else
        {
            _gameDocumentManager->NewDocument();
        }
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::Exit()
    {
        _window->SetShouldClose(ReadyToClose());
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::OpenDocument()
    {
        if (_gameDocumentManager->GetDocument()->IsDirty())
        {
            const auto sureOpen = Dialogs::Message(_localizationManager->Translate("StorytellerEditor", "You have unsaved changes, open other document anyway?").c_str(),
                _localizationManager->Translate("StorytellerEditor", "Open").c_str(), _window);

            if (sureOpen)
            {
                const auto filepath = Dialogs::OpenFile("JSON Files (*.json)\0*.json\0", _window);
                if (!filepath.empty())
                {
                    _recentList.remove(filepath);
                    _recentList.push_front(filepath);
                    _gameDocumentManager->OpenDocument(filepath);
                }
            }
        }
        else
        {
            const auto filepath = Dialogs::OpenFile("JSON Files (*.json)\0*.json\0", _window);
            if (!filepath.empty())
            {
                _recentList.remove(filepath);
                _recentList.push_front(filepath);
                _gameDocumentManager->OpenDocument(filepath);
            }
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
        const auto filepath = Dialogs::SaveFile("JSON Files (*.json)\0*.json\0", _window);
        _gameDocumentManager->Save(filepath);
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