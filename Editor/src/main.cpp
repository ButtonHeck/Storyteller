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

    Storyteller::LocalizationManager::Ptr localizationManager(new Storyteller::LocalizationManager());
    localizationManager->AddMessagesDomain(EDITOR_DOMAIN);
    localizationManager->Build("ru_RU.UTF-8");

    Storyteller::GlfwHelpers::SetupHints();
    std::shared_ptr<Storyteller::GlfwHelpers::UserData> userData = std::make_shared<Storyteller::GlfwHelpers::UserData>();

    auto window = glfwCreateWindow(userData->defaultWidth, userData->defaultHeight, 
        localizationManager->Translate(EDITOR_DOMAIN, "Storyteller Editor").c_str(), nullptr, nullptr);

    glfwSwapInterval(1);
    glfwSetWindowUserPointer(window, userData.get());
    glfwMakeContextCurrent(window);

    Storyteller::ImGuiHelpers::Initialize(window);

    Storyteller::GameDocument::Ptr gameDocument(new Storyteller::GameDocument(std::string()));
    Storyteller::GameDocumentSortFilterProxyView::Ptr gameDocumentProxy(new Storyteller::GameDocumentSortFilterProxyView(gameDocument));

    while (!glfwWindowShouldClose(window))
    {
        Storyteller::ImGuiHelpers::NewFrame();
        Storyteller::ImGuiHelpers::PrepareDockspace();
        Storyteller::ImGuiHelpers::Customize();

        // Menu bar
        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu(localizationManager->Translate(EDITOR_DOMAIN, "File").c_str()))
            {
                if (ImGui::MenuItem(localizationManager->Translate(EDITOR_DOMAIN, "Open").c_str()))
                {
                    std::string filepath = Storyteller::FileDialogs::OpenFile("JSON Files (*.json)\0*.json\0", window);
                    if (!filepath.empty())
                    {
                        std::cout << filepath << std::endl;
                        gameDocument.reset(new Storyteller::GameDocument(filepath));
                        gameDocumentProxy.reset(new Storyteller::GameDocumentSortFilterProxyView(gameDocument));
                    }
                }

                if (ImGui::MenuItem(localizationManager->Translate(EDITOR_DOMAIN, "Save").c_str()))
                {
                    gameDocument->Save();
                }

                if (ImGui::MenuItem(localizationManager->Translate(EDITOR_DOMAIN, "Save as...").c_str()))
                {
                    std::string filepath = Storyteller::FileDialogs::SaveFile("JSON Files (*.json)\0*.json\0", window);
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
            ImGuiWindowFlags mainFlags = gameDocument->IsDirty() ? ImGuiWindowFlags_UnsavedDocument : ImGuiWindowFlags();
            ImGui::Begin(localizationManager->Translate(gameDocument->GetGameName(), gameDocument->GetGameName()).c_str(), nullptr, mainFlags);

            static auto selectedTypeIndex = 0;

            if (ImGui::Button(localizationManager->Translate(EDITOR_DOMAIN, "Create").c_str()))
            {
                if (selectedTypeIndex == 0)
                {
                    gameDocumentProxy->AddObject(Storyteller::ObjectType::QuestObjectType);
                }
                else if (selectedTypeIndex == 1)
                {
                    gameDocumentProxy->AddObject(Storyteller::ObjectType::ActionObjectType);
                }
            }

            ImGui::SameLine();

            std::string typeItems[] = {
                localizationManager->Translate(STORYTELLER_DOMAIN, Storyteller::ObjectTypeToString(Storyteller::ObjectType::QuestObjectType)),
                localizationManager->Translate(STORYTELLER_DOMAIN, Storyteller::ObjectTypeToString(Storyteller::ObjectType::ActionObjectType)),
            };

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

            ImGui::Separator();
            ImGui::Text(localizationManager->Translate(EDITOR_DOMAIN, "Filters").c_str());
            static bool questObjectFilter = true;
            static bool actionObjectFilter = true;
            if (ImGui::Checkbox(localizationManager->Translate(STORYTELLER_DOMAIN, Storyteller::ObjectTypeToString(Storyteller::ObjectType::QuestObjectType)).c_str(), &questObjectFilter))
            {
                if (questObjectFilter)
                {
                    gameDocumentProxy->UpdateCache();
                }

                gameDocumentProxy->DoFilter(Storyteller::ObjectType::QuestObjectType, questObjectFilter);
            }

            ImGui::SameLine();
            if (ImGui::Checkbox(localizationManager->Translate(STORYTELLER_DOMAIN, Storyteller::ObjectTypeToString(Storyteller::ObjectType::ActionObjectType)).c_str(), &actionObjectFilter))
            {
                if (actionObjectFilter)
                {
                    gameDocumentProxy->UpdateCache();
                }

                gameDocumentProxy->DoFilter(Storyteller::ObjectType::ActionObjectType, actionObjectFilter);
            }

            ImGui::Separator();
            ImGui::Text(localizationManager->Translate(EDITOR_DOMAIN, "Objects").c_str());
            const auto flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable 
                | ImGuiTableFlags_Hideable | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_Sortable;

            if (ImGui::BeginTable(localizationManager->Translate(EDITOR_DOMAIN, "Objects").c_str(), 3, flags))
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
                        gameDocumentProxy->DoSort(direction == ImGuiSortDirection_Ascending, Storyteller::GameDocumentSortFilterProxyView::Sorter::SortValue(tableSortSpec->ColumnIndex));
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
                    ImGui::Selectable(Storyteller::ObjectTypeToString(object->GetObjectType()).c_str(), &selected, ImGuiSelectableFlags_SpanAllColumns);
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
            ImGui::End();
        }


        // game objects properties
        {
            ImGui::Begin(localizationManager->Translate(EDITOR_DOMAIN, "Texts").c_str());
            ImGui::Text(localizationManager->Translate(EDITOR_DOMAIN, "Name").c_str());
            const auto selectedObject = gameDocumentProxy->GetSelectedObject();
            const auto selectedUuid = selectedObject ? selectedObject->GetUuid() : Storyteller::UUID::InvalidUuid;
            auto objectName = selectedObject ? selectedObject->GetName() : std::string();
            if (ImGui::InputText(std::string("##ObjectName").append(std::to_string(selectedUuid)).c_str(), &objectName) && selectedObject)
            {
                selectedObject->SetName(objectName);
            }

            ImGui::Text(localizationManager->Translate(EDITOR_DOMAIN, "Source text").c_str());
            const auto selectedTextObject = dynamic_cast<Storyteller::TextObject*>(selectedObject.get());
            auto sourceText = selectedTextObject ? selectedTextObject->GetText() : std::string();
            if (ImGui::InputTextMultiline(std::string("##ObjectText").append(std::to_string(selectedUuid)).c_str(), &sourceText, ImVec2(-FLT_MIN, ImGui::GetContentRegionAvail().y / 2.0)) && selectedTextObject)
            {
                selectedTextObject->SetText(sourceText);
            }

            ImGui::Text(localizationManager->Translate(EDITOR_DOMAIN, "Translation").c_str());
            auto sourceTextTranslation = selectedTextObject ? localizationManager->Translate(gameDocument->GetGameName(), sourceText, true) : std::string();
            ImGui::InputTextMultiline(std::string("##Translation").append(std::to_string(selectedUuid)).c_str(), &sourceTextTranslation, ImVec2(-FLT_MIN, ImGui::GetContentRegionAvail().y), ImGuiInputTextFlags_ReadOnly);

            ImGui::End();
        }

        ImGui::ShowDemoWindow();
        ImGui::End();

        Storyteller::ImGuiHelpers::Render();
        Storyteller::ImGuiHelpers::UpdateDocking();
        Storyteller::ImGuiHelpers::EndFrame();

        Storyteller::GlfwHelpers::EndFrame(window, userData->updateContinuously);
    }

    Storyteller::ImGuiHelpers::Shutdown();
    Storyteller::GlfwHelpers::Shutdown(window);

    return 0;
}