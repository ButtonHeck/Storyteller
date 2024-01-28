#include "editor_ui_compositor.h"
#include "dialogs.h"
#include "log.h"
#include "ui_utils.h"

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>

namespace Storyteller
{
    EditorUiCompositor::EditorUiCompositor(Window::Ptr window, LocalizationManager::Ptr localizationManager)
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
        ComposeLogPanel();

        if (_state.demoWindow)
        {
            ImGui::ShowDemoWindow();
        }
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposeMenu()
    {
        if (ImGui::BeginMenuBar())
        {
            ComposeMenuFile();

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
            ComposeMenuItemSave();
            ComposeMenuItemSaveAs();

            ImGui::Separator();

            ComposeMenuItemQuit();
            ComposeMenuItemDemoWindow();

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
                const auto sureNew = Dialogs::Message(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Game document is not saved, are you sure to create new document?").c_str(),
                    _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "New").c_str(), _window->GetGLFWWindow());

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
                const auto sureOpen = Dialogs::Message(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Game document is not saved, are you sure to open other document?").c_str(),
                    _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Open").c_str(), _window->GetGLFWWindow());

                if (sureOpen)
                {
                    const auto filepath = Dialogs::OpenFile("JSON Files (*.json)\0*.json\0", _window->GetGLFWWindow());
                    if (!filepath.empty())
                    {
                        _gameDocumentManager->NewDocument(filepath);
                    }
                }
            }
            else
            {
                const auto filepath = Dialogs::OpenFile("JSON Files (*.json)\0*.json\0", _window->GetGLFWWindow());
                if (!filepath.empty())
                {
                    _gameDocumentManager->NewDocument(filepath);
                }
            }
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
            const auto filepath = Dialogs::SaveFile("JSON Files (*.json)\0*.json\0", _window->GetGLFWWindow());
            _gameDocumentManager->Save(filepath);
        }
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposeMenuItemQuit()
    {
        if (ImGui::MenuItem(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Quit").c_str()))
        {
            if (_gameDocumentManager->GetDocument()->IsDirty())
            {
                const auto sureQuit = Dialogs::Message(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Game document is not saved, are you sure to exit?").c_str(),
                    _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Quit").c_str(), _window->GetGLFWWindow());

                if (sureQuit)
                {
                    _window->SetShouldClose(true);
                }
            }
            else
            {
                _window->SetShouldClose(true);
            }
        }
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposeMenuItemDemoWindow()
    {
        if (ImGui::MenuItem("Demo window"))
        {
            _state.demoWindow = !_state.demoWindow;
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
        const auto document = _gameDocumentManager->GetDocument();

        ImGui::SeparatorText(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Game document").c_str());

        {
            UiUtils::ItemWidthGuard guard(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Name").c_str()).x);
            auto gameName = document->GetGameName();
            if (ImGui::InputText(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Name").c_str(), &gameName, ImGuiInputTextFlags_EnterReturnsTrue))
            {
                document->SetGameName(gameName);
            }
        }

        if (ImGui::Button(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Create translations file...").c_str()))
        {
            const auto filepath = Dialogs::SaveFile("Text Files (*.txt)\0*.txt\0", _window->GetGLFWWindow());
            _localizationManager->CreateTranslations(document, filepath);
        }
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposeGameDocumentPanelObjectsManagement()
    {
        const auto proxy = _gameDocumentManager->GetProxy();

        ImGui::SeparatorText(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Objects management").c_str());

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
            UiUtils::ItemWidthGuard guard(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Type").c_str()).x);
            if (ImGui::BeginCombo(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Type").c_str(), typeItems[_state.selectedTypeIndex].c_str()))
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

        {
            UiUtils::DisableGuard guard(!proxy->GetSelectedObject().get());
            if (ImGui::Button(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Remove").c_str()))
            {
                proxy->RemoveSelected();
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
        const auto proxy = _gameDocumentManager->GetProxy();

        ImGui::SeparatorText(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Objects").c_str());

        const auto objectsTableFlags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable
            | ImGuiTableFlags_ScrollY | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_Sortable;

        if (ImGui::BeginTable(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Objects").c_str(), 3, objectsTableFlags))
        {
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
                    tableSortSpec->ColumnIndex;
                    proxy->DoSort(direction == ImGuiSortDirection_Ascending, GameDocumentSortFilterProxyView::Sorter::SortValue(tableSortSpec->ColumnIndex));
                    sortSpecs->SpecsDirty = false;
                }
            }

            auto objects = proxy->GetObjects();
            for (auto row = 0; row < objects.size(); row++)
            {
                ImGui::TableNextRow();

                const auto object = objects[row];
                const auto consistent = object->IsConsistent();
                const auto rowColor = consistent ? ImVec4(1.0f, 1.0f, 1.0f, 1.0f) : ImVec4(1.0f, 0.5f, 0.5f, 1.0f);
                auto selected = proxy->IsSelected(object->GetUuid());

                {
                    UiUtils::StyleColorGuard guard({ {ImGuiCol_Text, rowColor} });

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
        const auto proxy = _gameDocumentManager->GetProxy();
        const auto selectedObject = proxy->GetSelectedObject();

        if (ImGui::Begin(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Texts").c_str(), nullptr))
        {
            ComposePropertiesPanelCommon();

            ImGui::SeparatorText(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Properties").c_str());
            const auto selectedObjectType = selectedObject ? selectedObject->GetObjectType() : ObjectType::ErrorObjectType;
            if (selectedObjectType == ObjectType::QuestObjectType)
            {
                ComposePropertiesPanelQuestObject();
            }
            else if (selectedObjectType == ObjectType::ActionObjectType)
            {
                ComposePropertiesPanelActionObject();
            }

            ImGui::End();
        }
    }

    void EditorUiCompositor::ComposePropertiesPanelCommon()
    {
        const auto proxy = _gameDocumentManager->GetProxy();
        const auto selectedObject = proxy->GetSelectedObject();
        const auto selectedUuid = selectedObject ? selectedObject->GetUuid() : Storyteller::UUID::InvalidUuid;
        const auto uuidString = std::to_string(selectedUuid);

        ImGui::Text(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Name").c_str());

        {
            UiUtils::ItemWidthGuard guard(-FLT_MIN);
            auto objectName = selectedObject ? selectedObject->GetName() : std::string();
            if (ImGui::InputText(std::string("##ObjectName").append(uuidString).c_str(), &objectName, ImGuiInputTextFlags_EnterReturnsTrue) && selectedObject)
            {
                selectedObject->SetName(objectName);
            }
        }

        const auto availableHeight = ImGui::GetContentRegionAvail().y;

        ImGui::Text(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Source text").c_str());
        const auto selectedTextObject = dynamic_cast<TextObject*>(selectedObject.get());
        auto sourceText = selectedTextObject ? selectedTextObject->GetText() : std::string();
        if (ImGui::InputTextMultiline(std::string("##ObjectText").append(uuidString).c_str(), &sourceText, ImVec2(-FLT_MIN, availableHeight / 4.0), ImGuiInputTextFlags_EnterReturnsTrue) && selectedTextObject)
        {
            selectedTextObject->SetText(sourceText);
        }

        ImGui::Text(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Translation").c_str());
        auto sourceTextTranslation = selectedTextObject ? _localizationManager->Translate(_gameDocumentManager->GetDocument()->GetGameName(), sourceText, true) : std::string();
        ImGui::InputTextMultiline(std::string("##Translation").append(uuidString).c_str(), &sourceTextTranslation, ImVec2(-FLT_MIN, availableHeight / 4.0), ImGuiInputTextFlags_ReadOnly);
    }
    //--------------------------------------------------------------------------

    void EditorUiCompositor::ComposePropertiesPanelQuestObject()
    {
        const auto proxy = _gameDocumentManager->GetProxy();
        const auto selectedObject = proxy->GetSelectedObject();
        const auto selectedUuid = selectedObject ? selectedObject->GetUuid() : Storyteller::UUID::InvalidUuid;

        auto selectedQuestObject = dynamic_cast<QuestObject*>(selectedObject.get());
        const auto allActionObjects = proxy->GetObjects(ObjectType::ActionObjectType, true);

        const auto entryPointObject = proxy->GetEntryPoint();
        auto isEntryPoint = entryPointObject ? (entryPointObject->GetUuid() == selectedObject->GetUuid()) : false;
        if (ImGui::Checkbox(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Entry point").c_str(), &isEntryPoint) && selectedObject)
        {
            proxy->SetEntryPoint(selectedUuid);
        }

        if (_state.selectedActionIndex >= allActionObjects.size())
        {
            _state.selectedActionIndex = 0;
        }

        auto isFinal = selectedQuestObject->IsFinal();
        ImGui::SameLine();
        if (ImGui::Checkbox(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Final").c_str(), &isFinal) && selectedObject)
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
            UiUtils::ItemWidthGuard guard(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Action name").c_str()).x);
            if (ImGui::BeginCombo(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Action name").c_str(), allActionObjects.empty() ? "" : allActionObjects[_state.selectedActionIndex]->GetName().c_str()))
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
            if (ImGui::Button(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Remove").c_str()) && !questObjectActions.empty())
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

    void EditorUiCompositor::ComposePropertiesPanelActionObject()
    {
        const auto proxy = _gameDocumentManager->GetProxy();
        const auto selectedObject = proxy->GetSelectedObject();
        const auto selectedUuid = selectedObject ? selectedObject->GetUuid() : Storyteller::UUID::InvalidUuid;

        auto selectedActionObject = dynamic_cast<ActionObject*>(selectedObject.get());
        const auto allQuestObjects = proxy->GetObjects(ObjectType::QuestObjectType, true);

        if (_state.selectedQuestIndex >= allQuestObjects.size())
        {
            _state.selectedQuestIndex = 0;
        }

        if (ImGui::Button(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Set target").c_str()))
        {
            selectedActionObject->SetTargetUuid(allQuestObjects.at(_state.selectedQuestIndex)->GetUuid());
        }

        ImGui::SameLine();

        {
            UiUtils::ItemWidthGuard guard(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Quest stage name").c_str()).x);
            if (ImGui::BeginCombo(_localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Quest stage name").c_str(), allQuestObjects.empty() ? "" : allQuestObjects[_state.selectedQuestIndex]->GetName().c_str()))
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
        ImGui::Text(targetObject ? targetObject->GetName().c_str() : _localizationManager->Translate(STRTLR_TR_DOMAIN_EDITOR, "Not set or does not exist").c_str());
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