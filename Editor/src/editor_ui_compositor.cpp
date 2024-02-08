#include "editor_ui_compositor.h"
#include "ui_utils.h"
#include "Storyteller/log.h"
#include "Storyteller/dialogs.h"

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

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

        const auto sureQuit = Dialogs::Message(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "You have unsaved changes, quit anyway?").c_str(),
            _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Quit").c_str(), _window);

        return sureQuit;
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::SaveSettings(Ptr<Settings> settings) const
    {
        settings->StartSaveGroup("EditorUiCompositor");
        settings->SaveBool("Log", _state.logPanel);
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
        if (ImGui::BeginMenu(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "File").c_str()))
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
        if (ImGui::BeginMenu(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "View").c_str()))
        {
            ComposeMenuItemLog();
            ComposeMenuItemFullscreen();

            ImGui::EndMenu();
        }
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposeMenuItemNew()
    {
        if (ImGui::MenuItem(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "New").c_str()))
        {
            if (_gameDocumentManager->GetDocument()->IsDirty())
            {
                const auto sureNew = Dialogs::Message(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "You have unsaved changes, are you sure to create new document?").c_str(),
                    _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "New").c_str(), _window);

                if (sureNew)
                {
                    _gameDocumentManager->NewDocument(std::string());
                }
            }
            else
            {
                _gameDocumentManager->NewDocument(std::string());
            }
        }
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposeMenuItemOpen()
    {
        if (ImGui::MenuItem(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Open").c_str()))
        {
            if (_gameDocumentManager->GetDocument()->IsDirty())
            {
                const auto sureOpen = Dialogs::Message(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "You have unsaved changes, are you sure to open other document?").c_str(),
                    _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Open").c_str(), _window);

                if (sureOpen)
                {
                    const auto filepath = Dialogs::OpenFile("JSON Files (*.json)\0*.json\0", _window);
                    if (!filepath.empty())
                    {
                        _recentList.remove(filepath);
                        _recentList.push_front(filepath);
                        _gameDocumentManager->NewDocument(filepath);
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
                    _gameDocumentManager->NewDocument(filepath);
                }
            }
        }
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposeMenuItemOpenRecent()
    {
        UiUtils::DisableGuard guard(_recentList.empty());
        if (ImGui::BeginMenu(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Open recent").c_str()))
        {
            auto recentToUpdate = std::string();
            for (const auto& recent : _recentList)
            {
                if (ImGui::MenuItem(recent.c_str()))
                {
                    if (_gameDocumentManager->GetDocument()->IsDirty())
                    {
                        const auto sureOpen = Dialogs::Message(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "You have unsaved changes, are you sure to open other document?").c_str(),
                            _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Open").c_str(), _window);

                        if (sureOpen)
                        {
                            recentToUpdate = recent;
                            _gameDocumentManager->NewDocument(recent); //todo: missing/malformed json fix
                        }
                    }
                    else
                    {
                        recentToUpdate = recent;
                        _gameDocumentManager->NewDocument(recent); //todo: missing/malformed json fix
                    }
                }
            }

            if (!recentToUpdate.empty())
            {
                _recentList.remove(recentToUpdate);
                _recentList.push_front(recentToUpdate);
            }

            if (!_recentList.empty())
            {
                ImGui::Separator();
                if (ImGui::MenuItem(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Clear").c_str()))
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
        if (ImGui::MenuItem(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Save").c_str()))
        {
            _gameDocumentManager->Save();
        }
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposeMenuItemSaveAs()
    {
        if (ImGui::MenuItem(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Save as...").c_str()))
        {
            const auto filepath = Dialogs::SaveFile("JSON Files (*.json)\0*.json\0", _window);
            _gameDocumentManager->Save(filepath);
        }
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposeMenuItemQuit()
    {
        if (ImGui::MenuItem(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Quit").c_str()))
        {
            _window->SetShouldClose(ReadyToClose());
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
        ImGui::MenuItem(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Log").c_str(), nullptr, &_state.logPanel);
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposeMenuItemFullscreen()
    {
        const auto screenMode = _window->GetScreenMode();
        auto isFullscreen = screenMode == Window::WindowedFullscreenMode;
        if (ImGui::MenuItem(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Fullscreen").c_str(), nullptr, &isFullscreen))
        {
            _window->SetScreenMode(isFullscreen ? Window::WindowedFullscreenMode : Window::WindowedMode);
        }        
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposeGameDocumentPanel()
    {
        const auto mainFlags = _gameDocumentManager->GetDocument()->IsDirty() ? ImGuiWindowFlags_UnsavedDocument : ImGuiWindowFlags();
        if (ImGui::Begin(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Game").c_str(), nullptr, mainFlags))
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
        ImGui::SeparatorText(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Game document").c_str());

        const auto document = _gameDocumentManager->GetDocument();

        {
            const auto title = _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Name");
            UiUtils::ItemWidthGuard guard(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(title.c_str()).x);
            auto gameName = document->GetGameName();
            if (ImGui::InputText(title.c_str(), &gameName, ImGuiInputTextFlags_EnterReturnsTrue))
            {
                document->SetGameName(gameName);
            }
        }

        if (ImGui::Button(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Create translations file...").c_str()))
        {
            const auto filepath = Dialogs::SaveFile("Text Files (*.txt)\0*.txt\0", _window);
            _localizationManager->CreateTranslations(document, filepath);
        }
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposeGameDocumentPanelObjectsManagement()
    {
        ImGui::SeparatorText(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Objects management").c_str());

        const auto proxy = _gameDocumentManager->GetProxy();

        if (ImGui::Button(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Create").c_str()))
        {
            proxy->AddObject(ObjectType(_state.selectedTypeIndex));
        }

        ImGui::SameLine();

        std::string typeItems[] = {
            _localizationManager->Translate(STRTLR_TR_DOMAIN_ENGINE, ObjectTypeToString(ObjectType::QuestObjectType)),
            _localizationManager->Translate(STRTLR_TR_DOMAIN_ENGINE, ObjectTypeToString(ObjectType::ActionObjectType)),
        };

        {
            const auto title = _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Type");
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
        ImGui::SeparatorText(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Filters").c_str());

        ComposeGameDocumentPanelFilterCheckbox(ObjectType::QuestObjectType, _state.questObjectFilter);
        ImGui::SameLine();
        ComposeGameDocumentPanelFilterCheckbox(ObjectType::ActionObjectType, _state.actionObjectFilter);
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
        ImGui::SeparatorText(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Objects").c_str());

        const auto proxy = _gameDocumentManager->GetProxy();

        const auto objectsTableFlags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable
            | ImGuiTableFlags_ScrollY | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_Sortable;

        if (ImGui::BeginTable(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Objects").c_str(), 4, objectsTableFlags))
        {
            ImGui::TableSetupColumn(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Action").c_str(), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_NoSort);
            ImGui::TableSetupColumn(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Type").c_str(), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultSort);
            ImGui::TableSetupColumn(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "UUID").c_str(), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultSort);
            ImGui::TableSetupColumn(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Name").c_str(), ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_DefaultSort);
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
                    if (ImGui::SmallButton(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Delete").c_str()))
                    {
                        proxy->RemoveObject(object->GetUuid());
                        objects = proxy->GetObjects();
                    }
                }

                {
                    UiUtils::StyleColorGuard guard({ {ImGuiCol_Text, consistent ? ImVec4(1.0f, 1.0f, 1.0f, 1.0f) : ImVec4(1.0f, 0.5f, 0.5f, 1.0f)} });

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
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposePropertiesPanel()
    {
        if (ImGui::Begin(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Properties").c_str(), nullptr))
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

        ImGui::SeparatorText(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Name").c_str());

        const auto uuidString = std::to_string(selectedObject->GetUuid());

        UiUtils::ItemWidthGuard guard(-FLT_MIN);
        auto objectName = selectedObject->GetName();
        const auto oldObjectName = objectName;
        if (ImGui::InputText(std::string("##ObjectName").append(uuidString).c_str(), &objectName, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            if (oldObjectName != objectName && !_gameDocumentManager->GetProxy()->SetObjectName(selectedObject->GetUuid(), objectName))
            {
                Dialogs::Message(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Name already exists").c_str(), 
                    _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Warning").c_str(), _window, Dialogs::OkButtons);
            }
        }
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposePropertiesPanelTexts(Ptr<BasicObject> selectedObject)
    {
        assert(selectedObject);

        ImGui::SeparatorText(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Source text").c_str());

        const auto uuidString = std::to_string(selectedObject->GetUuid());
        const auto textPanelHeight = ImGui::GetContentRegionAvail().y / 4.0f;
        const auto selectedTextObject = dynamic_cast<TextObject*>(selectedObject.get());
        auto sourceText = selectedTextObject ? selectedTextObject->GetText() : std::string();

        if (ImGui::InputTextMultiline(std::string("##ObjectText").append(uuidString).c_str(), &sourceText, ImVec2(-FLT_MIN, textPanelHeight), ImGuiInputTextFlags_EnterReturnsTrue) && selectedTextObject)
        {
            selectedTextObject->SetText(sourceText);
        }

        ImGui::SeparatorText(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Translation").c_str());

        auto sourceTextTranslation = selectedTextObject ? _localizationManager->Translate(_gameDocumentManager->GetDocument()->GetGameName(), sourceText, true) : std::string();
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
        const auto allActionObjects = proxy->GetObjects(ObjectType::ActionObjectType, true);

        const auto entryPointObject = proxy->GetEntryPoint();
        auto isEntryPoint = entryPointObject ? (entryPointObject->GetUuid() == selectedUuid) : false;

        if (ImGui::Checkbox(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Entry point").c_str(), &isEntryPoint))
        {
            proxy->SetEntryPoint(selectedUuid);
        }

        if (_state.selectedActionIndex >= allActionObjects.size())
        {
            _state.selectedActionIndex = 0;
        }

        auto isFinal = selectedQuestObject->IsFinal();
        ImGui::SameLine();
        if (ImGui::Checkbox(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Final").c_str(), &isFinal))
        {
            selectedQuestObject->SetFinal(isFinal);
        }

        {
            UiUtils::DisableGuard guard(allActionObjects.empty());
            if (ImGui::Button(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Add action").c_str()))
            {
                selectedQuestObject->AddAction(allActionObjects.at(_state.selectedActionIndex)->GetUuid());
            }
        }

        ImGui::SameLine();

        {
            const auto title = _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Action name");
            UiUtils::ItemWidthGuard guard(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(title.c_str()).x);
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

        {
            UiUtils::DisableGuard guard(questObjectActions.empty());
            if (ImGui::Button(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Remove").c_str()))
            {
                selectedQuestObject->RemoveAction(questObjectActions.at(_state.selectedChildActionIndex));
            }
        }

        const auto actionsTableFlags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable 
            | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_ScrollY;
        if (ImGui::BeginTable(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Objects's actions").c_str(), 3, actionsTableFlags))
        {
            ImGui::TableSetupColumn(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "UUID").c_str(), ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Name").c_str(), ImGuiTableColumnFlags_WidthFixed);
            ImGui::TableSetupColumn(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Text").c_str(), ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupScrollFreeze(0, 1);
            ImGui::TableHeadersRow();

            for (auto row = 0; row < questObjectActions.size(); row++)
            {
                const auto object = proxy->GetBasicObject(questObjectActions[row]);
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

        ImGui::SeparatorText(_localizationManager->Translate(STRTLR_TR_DOMAIN_ENGINE, ObjectTypeToString(selectedObject->GetObjectType())).c_str());

        const auto proxy = _gameDocumentManager->GetProxy();
        const auto selectedActionObject = dynamic_cast<ActionObject*>(selectedObject.get());
        const auto allQuestObjects = proxy->GetObjects(ObjectType::QuestObjectType, true);

        if (_state.selectedQuestIndex >= allQuestObjects.size())
        {
            _state.selectedQuestIndex = 0;
        }

        {
            UiUtils::DisableGuard guard(allQuestObjects.empty());
            if (ImGui::Button(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Set target").c_str()))
            {
                selectedActionObject->SetTargetUuid(allQuestObjects.at(_state.selectedQuestIndex)->GetUuid());
            }
        }

        ImGui::SameLine();

        {
            const auto title = _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Quest stage name");
            UiUtils::ItemWidthGuard guard(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(title.c_str()).x);
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

        if (ImGui::Button(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Clear target").c_str()))
        {
            selectedActionObject->SetTargetUuid(Storyteller::UUID::InvalidUuid);
        }

        ImGui::Text(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Current target name: ").c_str());
        ImGui::SameLine();
        const auto targetObject = proxy->GetBasicObject(selectedActionObject->GetTargetUuid());
        ImGui::Text(targetObject ? std::string("[").append(targetObject->GetName()).append("]").c_str() : _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Not set or does not exist").c_str());
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposeLogPanel()
    {
        UiUtils::StyleColorGuard colorGuard({ {ImGuiCol_FrameBg, ImVec4(0, 0, 0, 0)} });
        UiUtils::StyleVarGuard varGuard({ {ImGuiStyleVar_FrameBorderSize, 0.0f}, {ImGuiStyleVar_WindowPadding, ImVec2(1.0f, 1.0f)} });

        ImGui::Begin(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Log").c_str(), nullptr);
        auto logDataStr = std::string(Log::StringLogOutput());
        ImGui::InputTextMultiline("##Log", &logDataStr, ImVec2(-FLT_MIN, -FLT_MIN), ImGuiInputTextFlags_ReadOnly);
        ImGui::End();
    }
    //--------------------------------------------------------------------------
}