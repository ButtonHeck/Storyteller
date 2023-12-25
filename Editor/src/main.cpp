#include "storyteller.h"
#include "glfw_helpers.h"
#include "imgui_helpers.h"
#include "file_dialogs.h"

#include <imgui.h>
#include <misc/cpp/imgui_stdlib.h>
#include <backends/imgui_impl_glfw.h>
#include <backends/imgui_impl_opengl3.h>
#include <boost/locale.hpp>
#include <memory>
#include <iostream>

#define STORYTELLER_DOMAIN "Storyteller"
#define EDITOR_DOMAIN "StorytellerEditor"

int main()
{
    if (!glfwInit())
    {
        return -1;
    }

    using namespace Storyteller;

    LocalizationManager::Ptr localizationManager(new LocalizationManager());
    localizationManager->AddMessagesDomain(EDITOR_DOMAIN);
    localizationManager->Build("ru_RU.UTF-8");

    GlfwHelpers::SetupHints();
    std::shared_ptr<GlfwHelpers::UserData> userData = std::make_shared<GlfwHelpers::UserData>();

    auto window = glfwCreateWindow(userData->defaultWidth, userData->defaultHeight, 
        localizationManager->Translate(EDITOR_DOMAIN, "Storyteller Editor").c_str(), nullptr, nullptr);

    glfwSwapInterval(1);
    glfwSetWindowUserPointer(window, userData.get());
    glfwMakeContextCurrent(window);

    ImGuiHelpers::Initialize(window);

    GameDocument::Ptr gameDocument(new GameDocument(std::string()));
    GameDocumentSortFilterProxyView::Ptr gameDocumentProxy(new GameDocumentSortFilterProxyView(gameDocument));

    while (!glfwWindowShouldClose(window))
    {
        ImGuiHelpers::NewFrame();
        ImGuiHelpers::PrepareDockspace();
        ImGuiHelpers::Customize();

        // Menu bar
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu(localizationManager->Translate(EDITOR_DOMAIN, "File").c_str()))
            {
                if (ImGui::MenuItem(localizationManager->Translate(EDITOR_DOMAIN, "Open").c_str()))
                {
                    std::string filepath = FileDialogs::OpenFile("JSON Files (*.json)\0*.json\0", window);
                    if (!filepath.empty())
                    {
                        std::cout << filepath << std::endl;
                        gameDocument.reset(new GameDocument(filepath));
                        gameDocumentProxy.reset(new GameDocumentSortFilterProxyView(gameDocument));
                    }
                }

                if (ImGui::MenuItem(localizationManager->Translate(EDITOR_DOMAIN, "Save").c_str()))
                {
                    gameDocument->Save();
                }

                if (ImGui::MenuItem(localizationManager->Translate(EDITOR_DOMAIN, "Save as...").c_str()))
                {
                    std::string filepath = FileDialogs::SaveFile("JSON Files (*.json)\0*.json\0", window);
                    gameDocument->Save(filepath);
                }

                if (ImGui::MenuItem(localizationManager->Translate(EDITOR_DOMAIN, "Quit").c_str()))
                {
                    glfwSetWindowShouldClose(window, true);
                }

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        // game document
        {
            const auto mainFlags = gameDocument->IsDirty() ? ImGuiWindowFlags_UnsavedDocument : ImGuiWindowFlags();
            ImGui::Begin(localizationManager->Translate(EDITOR_DOMAIN, "Game").c_str(), nullptr, mainFlags);

            ImGui::SeparatorText(localizationManager->Translate(EDITOR_DOMAIN, "Game name").c_str());
            auto gameName = gameDocument->GetGameName();
            ImGui::PushItemWidth(-FLT_MIN);
            if (ImGui::InputText(localizationManager->Translate(EDITOR_DOMAIN, "Game name").c_str(), &gameName, ImGuiInputTextFlags_EnterReturnsTrue))
            {
                gameDocument->SetGameName(gameName);
            }
            ImGui::PopItemWidth();


            ImGui::SeparatorText(localizationManager->Translate(EDITOR_DOMAIN, "Objects management").c_str());
            static auto selectedTypeIndex = 0;
            if (ImGui::Button(localizationManager->Translate(EDITOR_DOMAIN, "Create").c_str()))
            {
                if (selectedTypeIndex == 0)
                {
                    gameDocumentProxy->AddObject(ObjectType::QuestObjectType);
                }
                else if (selectedTypeIndex == 1)
                {
                    gameDocumentProxy->AddObject(ObjectType::ActionObjectType);
                }
            }

            ImGui::SameLine();

            std::string typeItems[] = {
                localizationManager->Translate(STORYTELLER_DOMAIN, ObjectTypeToString(ObjectType::QuestObjectType)),
                localizationManager->Translate(STORYTELLER_DOMAIN, ObjectTypeToString(ObjectType::ActionObjectType)),
            };

            ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(localizationManager->Translate(EDITOR_DOMAIN, "Type").c_str()).x);
            if (ImGui::BeginCombo(localizationManager->Translate(EDITOR_DOMAIN, "Type").c_str(), typeItems[selectedTypeIndex].c_str()))
            {
                for (auto typeIndex = 0; typeIndex < 2; typeIndex++)
                {
                    const auto selected = selectedTypeIndex == typeIndex;
                    if (ImGui::Selectable(typeItems[typeIndex].c_str(), selected))
                    {
                        selectedTypeIndex = typeIndex;
                    }

                    if (selected)
                    {
                        ImGui::SetItemDefaultFocus();
                    }
                }

                ImGui::EndCombo();
            }
            ImGui::PopItemWidth();

            const auto hasSelection = gameDocumentProxy->GetSelectedObject();
            if (!hasSelection)
            {
                ImGui::BeginDisabled();
            }
            if (ImGui::Button(localizationManager->Translate(EDITOR_DOMAIN, "Remove").c_str()))
            {
                gameDocumentProxy->RemoveSelected();
            }
            if (!hasSelection)
            {
                ImGui::EndDisabled();
            }


            ImGui::SeparatorText(localizationManager->Translate(EDITOR_DOMAIN, "Filters").c_str());
            static bool questObjectFilter = true;
            static bool actionObjectFilter = true;
            if (ImGui::Checkbox(localizationManager->Translate(STORYTELLER_DOMAIN, ObjectTypeToString(ObjectType::QuestObjectType)).c_str(), &questObjectFilter))
            {
                if (questObjectFilter)
                {
                    gameDocumentProxy->UpdateCache();
                }

                gameDocumentProxy->DoFilter(ObjectType::QuestObjectType, questObjectFilter);
            }

            ImGui::SameLine();
            if (ImGui::Checkbox(localizationManager->Translate(STORYTELLER_DOMAIN, ObjectTypeToString(ObjectType::ActionObjectType)).c_str(), &actionObjectFilter))
            {
                if (actionObjectFilter)
                {
                    gameDocumentProxy->UpdateCache();
                }

                gameDocumentProxy->DoFilter(ObjectType::ActionObjectType, actionObjectFilter);
            }


            ImGui::SeparatorText(localizationManager->Translate(EDITOR_DOMAIN, "Objects").c_str());
            {
                ImGui::BeginChild(localizationManager->Translate(EDITOR_DOMAIN, "Objects table").c_str());
                const auto objectsTableFlags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable
                    | ImGuiTableFlags_Hideable | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_Sortable;

                if (ImGui::BeginTable(localizationManager->Translate(EDITOR_DOMAIN, "Objects").c_str(), 3, objectsTableFlags))
                {
                    ImGui::TableSetupColumn(localizationManager->Translate(EDITOR_DOMAIN, "Type").c_str(), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultSort);
                    ImGui::TableSetupColumn(localizationManager->Translate(EDITOR_DOMAIN, "UUID").c_str(), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultSort);
                    ImGui::TableSetupColumn(localizationManager->Translate(EDITOR_DOMAIN, "Name").c_str(), ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_DefaultSort);
                    ImGui::TableSetupScrollFreeze(0, 1);
                    ImGui::TableHeadersRow();

                    if (auto* sortSpecs = ImGui::TableGetSortSpecs())
                    {
                        if (sortSpecs->SpecsDirty)
                        {
                            const auto tableSortSpec = sortSpecs->Specs;
                            const auto direction = tableSortSpec->SortDirection;
                            tableSortSpec->ColumnIndex;
                            gameDocumentProxy->DoSort(direction == ImGuiSortDirection_Ascending, GameDocumentSortFilterProxyView::Sorter::SortValue(tableSortSpec->ColumnIndex));
                            sortSpecs->SpecsDirty = false;
                        }
                    }

                    auto objects = gameDocumentProxy->GetObjects();
                    for (auto row = 0; row < objects.size(); row++)
                    {
                        ImGui::TableNextRow();

                        const auto object = objects[row];
                        const auto consistent = object->IsConsistent();
                        const auto rowColor = consistent ? ImVec4(1.0f, 1.0f, 1.0f, 1.0f) : ImVec4(1.0f, 0.5f, 0.5f, 1.0f);
                        auto selected = gameDocumentProxy->IsSelected(object->GetUuid());

                        ImGui::TableNextColumn();
                        ImGui::PushStyleColor(ImGuiCol_Text, rowColor);
                        ImGui::Selectable(localizationManager->Translate(STORYTELLER_DOMAIN, ObjectTypeToString(object->GetObjectType())).c_str(), &selected, ImGuiSelectableFlags_SpanAllColumns);
                        ImGui::PopStyleColor();
                        if (ImGui::IsItemClicked(0))
                        {
                            gameDocumentProxy->Select(object->GetUuid());
                        }

                        ImGui::TableNextColumn();
                        ImGui::TextColored(rowColor, std::to_string(object->GetUuid()).c_str());

                        ImGui::TableNextColumn();
                        ImGui::TextColored(rowColor, object->GetName().c_str());
                    }

                    ImGui::EndTable();
                }

                ImGui::EndChild();
            }
            
            ImGui::End();
        }


        // game objects properties
        {
            ImGui::Begin(localizationManager->Translate(EDITOR_DOMAIN, "Texts").c_str(), nullptr);
            ImGui::Text(localizationManager->Translate(EDITOR_DOMAIN, "Name").c_str());

            const auto selectedObject = gameDocumentProxy->GetSelectedObject();
            const auto selectedUuid = selectedObject ? selectedObject->GetUuid() : UUID::InvalidUuid;
            const auto uuidString = std::to_string(selectedUuid);

            auto objectName = selectedObject ? selectedObject->GetName() : std::string();
            ImGui::PushItemWidth(-FLT_MIN);
            if (ImGui::InputText(std::string("##ObjectName").append(uuidString).c_str(), &objectName, ImGuiInputTextFlags_EnterReturnsTrue) && selectedObject)
            {
                selectedObject->SetName(objectName);
            }
            ImGui::PopItemWidth();

            const auto availableHeight = ImGui::GetContentRegionAvail().y;

            ImGui::Text(localizationManager->Translate(EDITOR_DOMAIN, "Source text").c_str());
            const auto selectedTextObject = dynamic_cast<TextObject*>(selectedObject.get());
            auto sourceText = selectedTextObject ? selectedTextObject->GetText() : std::string();
            if (ImGui::InputTextMultiline(std::string("##ObjectText").append(uuidString).c_str(), &sourceText, ImVec2(-FLT_MIN, availableHeight / 3.0), ImGuiInputTextFlags_EnterReturnsTrue) && selectedTextObject)
            {
                selectedTextObject->SetText(sourceText);
            }

            ImGui::Text(localizationManager->Translate(EDITOR_DOMAIN, "Translation").c_str());
            auto sourceTextTranslation = selectedTextObject ? localizationManager->Translate(gameDocument->GetGameName(), sourceText, true) : std::string();
            ImGui::InputTextMultiline(std::string("##Translation").append(uuidString).c_str(), &sourceTextTranslation, ImVec2(-FLT_MIN, availableHeight / 3.0), ImGuiInputTextFlags_ReadOnly);


            ImGui::SeparatorText(localizationManager->Translate(EDITOR_DOMAIN, "Properties").c_str());
            const auto selectedObjectType = selectedObject ? selectedObject->GetObjectType() : ObjectType::ErrorObjectType;
            if (selectedObjectType == ObjectType::QuestObjectType)
            {
                auto selectedQuestObject = dynamic_cast<QuestObject*>(selectedObject.get());
                const auto allActionObjects = gameDocumentProxy->GetObjects(ObjectType::ActionObjectType, true);

                const auto entryPointObject = gameDocumentProxy->GetEntryPoint();
                auto isEntryPoint = entryPointObject ? (entryPointObject->GetUuid() == selectedObject->GetUuid()) : false;
                if (ImGui::Checkbox(localizationManager->Translate(EDITOR_DOMAIN, "Entry point").c_str(), &isEntryPoint) && selectedObject)
                {
                    gameDocumentProxy->SetEntryPoint(selectedUuid);
                }

                static auto selectedActionIndex = 0;
                if (selectedActionIndex >= allActionObjects.size())
                {
                    selectedActionIndex = 0;
                }

                auto isFinal = selectedQuestObject->IsFinal();
                ImGui::SameLine();
                if (ImGui::Checkbox(localizationManager->Translate(EDITOR_DOMAIN, "Final").c_str(), &isFinal) && selectedObject)
                {
                    selectedQuestObject->SetFinal(isFinal);
                }

                if (ImGui::Button(localizationManager->Translate(EDITOR_DOMAIN, "Add action").c_str()))
                {
                    selectedQuestObject->AddAction(allActionObjects.at(selectedActionIndex)->GetUuid());
                }

                ImGui::SameLine();
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(localizationManager->Translate(EDITOR_DOMAIN, "Action name").c_str()).x);
                if (ImGui::BeginCombo(localizationManager->Translate(EDITOR_DOMAIN, "Action name").c_str(), allActionObjects.empty() ? "" : allActionObjects[selectedActionIndex]->GetName().c_str()))
                {
                    for (auto actionIndex = 0; actionIndex < allActionObjects.size(); actionIndex++)
                    {
                        const auto selected = selectedActionIndex == actionIndex;
                        if (ImGui::Selectable(allActionObjects.at(actionIndex)->GetName().c_str(), selected))
                        {
                            selectedActionIndex = actionIndex;
                        }

                        if (selected)
                        {
                            ImGui::SetItemDefaultFocus();
                        }
                    }

                    ImGui::EndCombo();
                }
                ImGui::PopItemWidth();

                const auto questObjectActions = selectedQuestObject->GetActions();
                static auto selectedChildActionIndex = 0;
                if (selectedChildActionIndex >= questObjectActions.size())
                {
                    selectedChildActionIndex = 0;
                }

                if (questObjectActions.empty())
                {
                    ImGui::BeginDisabled();
                }
                if (ImGui::Button(localizationManager->Translate(EDITOR_DOMAIN, "Remove").c_str()) && !questObjectActions.empty())
                {
                    selectedQuestObject->RemoveAction(questObjectActions.at(selectedChildActionIndex));
                }
                if (questObjectActions.empty())
                {
                    ImGui::EndDisabled();
                }

                ImGui::BeginChild(localizationManager->Translate(EDITOR_DOMAIN, "Object's actions").c_str());
                const auto actionsTableFlags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_NoHostExtendX;
                if (ImGui::BeginTable(localizationManager->Translate(EDITOR_DOMAIN, "Objects's actions").c_str(), 3, actionsTableFlags))
                {
                    ImGui::TableSetupColumn(localizationManager->Translate(EDITOR_DOMAIN, "UUID").c_str(), ImGuiTableColumnFlags_WidthFixed);
                    ImGui::TableSetupColumn(localizationManager->Translate(EDITOR_DOMAIN, "Name").c_str(), ImGuiTableColumnFlags_WidthFixed);
                    ImGui::TableSetupColumn(localizationManager->Translate(EDITOR_DOMAIN, "Text").c_str(), ImGuiTableColumnFlags_WidthStretch);
                    ImGui::TableSetupScrollFreeze(0, 1);
                    ImGui::TableHeadersRow();

                    for (auto row = 0; row < questObjectActions.size(); row++)
                    {
                        const auto object = gameDocumentProxy->GetObject(questObjectActions[row]);
                        if (!object)
                        {
                            continue;
                        }

                        const auto actionObject = dynamic_cast<ActionObject*>(object.get());
                        if (!actionObject)
                        {
                            continue;
                        }

                        auto selected = selectedChildActionIndex == row;

                        ImGui::TableNextRow();

                        ImGui::TableNextColumn();
                        ImGui::Selectable(std::to_string(actionObject->GetUuid()).c_str(), &selected, ImGuiSelectableFlags_SpanAllColumns);
                        if (ImGui::IsItemClicked(0))
                        {
                            selectedChildActionIndex = row;
                        }

                        ImGui::TableNextColumn();
                        ImGui::Text(object->GetName().c_str());

                        ImGui::TableNextColumn();
                        ImGui::Text(localizationManager->Translate(gameDocument->GetGameName(), actionObject->GetText()).c_str());
                    }

                    ImGui::EndTable();
                }

                ImGui::EndChild();
            }
            else if (selectedObjectType == ObjectType::ActionObjectType)
            {
                auto selectedActionObject = dynamic_cast<ActionObject*>(selectedObject.get());
                const auto allQuestObjects = gameDocumentProxy->GetObjects(ObjectType::QuestObjectType, true);

                static auto selectedQuestIndex = 0;
                if (selectedQuestIndex >= allQuestObjects.size())
                {
                    selectedQuestIndex = 0;
                }

                if (ImGui::Button(localizationManager->Translate(EDITOR_DOMAIN, "Set target").c_str()))
                {
                    selectedActionObject->SetTargetUuid(allQuestObjects.at(selectedQuestIndex)->GetUuid());
                }

                ImGui::SameLine();
                ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x - ImGui::CalcTextSize(localizationManager->Translate(EDITOR_DOMAIN, "Quest stage name").c_str()).x);
                if (ImGui::BeginCombo(localizationManager->Translate(EDITOR_DOMAIN, "Quest stage name").c_str(), allQuestObjects.empty() ? "" : allQuestObjects[selectedQuestIndex]->GetName().c_str()))
                {
                    for (auto questIndex = 0; questIndex < allQuestObjects.size(); questIndex++)
                    {
                        const auto selected = selectedQuestIndex == questIndex;
                        if (ImGui::Selectable(allQuestObjects.at(questIndex)->GetName().c_str(), selected))
                        {
                            selectedQuestIndex = questIndex;
                        }

                        if (selected)
                        {
                            ImGui::SetItemDefaultFocus();
                        }
                    }
                    ImGui::EndCombo();
                }
                ImGui::PopItemWidth();

                if (ImGui::Button(localizationManager->Translate(EDITOR_DOMAIN, "Clear target").c_str()))
                {
                    selectedActionObject->SetTargetUuid(UUID::InvalidUuid);
                }

                ImGui::Text(localizationManager->Translate(EDITOR_DOMAIN, "Current target name: ").c_str());
                ImGui::SameLine();
                const auto targetObject = gameDocumentProxy->GetObject(selectedActionObject->GetTargetUuid());
                ImGui::Text(targetObject ? targetObject->GetName().c_str() : localizationManager->Translate(EDITOR_DOMAIN, "Not set or does not exist").c_str());
            }

            ImGui::End();
        }

        ImGui::ShowDemoWindow();
        ImGui::End();

        ImGuiHelpers::Render();
        ImGuiHelpers::UpdateDocking();
        ImGuiHelpers::EndFrame();

        GlfwHelpers::EndFrame(window, userData->updateContinuously);
    }

    ImGuiHelpers::Shutdown();
    GlfwHelpers::Shutdown(window);

    return 0;
}