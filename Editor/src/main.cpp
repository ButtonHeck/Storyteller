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

int main()
{
    boost::locale::generator gen;
    gen.add_messages_path(".");
    gen.add_messages_domain("test");
    std::locale::global(gen("ru_RU.UTF-8"));
    std::cout.imbue(std::locale());

    if (!glfwInit())
    {
        return -1;
    }

    Storyteller::GlfwHelpers::SetupHints();
    std::shared_ptr<Storyteller::GlfwHelpers::UserData> userData = std::make_shared<Storyteller::GlfwHelpers::UserData>();

    auto window = glfwCreateWindow(userData->defaultWidth, userData->defaultHeight, 
        boost::locale::translate("Storyteller Editor").str().c_str(), nullptr, nullptr);

    glfwSwapInterval(1);
    glfwSetWindowUserPointer(window, userData.get());
    glfwMakeContextCurrent(window);

    Storyteller::ImGuiHelpers::Initialize(window);

    auto textModel = Storyteller::TextObjectModel();

    while (!glfwWindowShouldClose(window))
    {
        Storyteller::ImGuiHelpers::NewFrame();
        Storyteller::ImGuiHelpers::PrepareDockspace();
        Storyteller::ImGuiHelpers::Customize();

        if (ImGui::BeginMenuBar())
        {
            if (ImGui::BeginMenu(boost::locale::translate("File").str().c_str()))
            {
                if (ImGui::MenuItem(boost::locale::translate("Open").str().c_str()))
                {
                    std::string filepath = Storyteller::FileDialogs::OpenFile("JSON Files (*.json)\0*.json\0", window);
                    if (!filepath.empty())
                    {
                        std::cout << filepath << std::endl;
                    }
                }

                if (ImGui::MenuItem(boost::locale::translate("Save").str().c_str()))
                {
                    // save
                }

                if (ImGui::MenuItem(boost::locale::translate("Save as...").str().c_str()))
                {
                    // save as
                }

                if (ImGui::MenuItem(boost::locale::translate("Quit").str().c_str()))
                {
                    glfwSetWindowShouldClose(window, true);
                }

                ImGui::EndMenu();
            }
            ImGui::EndMenuBar();
        }

        {
            ImGui::Begin(boost::locale::translate("Entities").str().c_str());

            if (ImGui::Button(boost::locale::translate("Create").str().c_str()))
            {
                textModel.Add(Storyteller::UUID());
            }

            ImGui::SameLine();
            if (ImGui::Button(boost::locale::translate("Remove").str().c_str()) && !textModel.IsSelected(0))
            {
                textModel.RemoveSelected();
            }

            ImGuiTableFlags flags = ImGuiTableFlags_SizingFixedFit | ImGuiTableFlags_RowBg | ImGuiTableFlags_Borders | ImGuiTableFlags_Resizable | ImGuiTableFlags_Hideable 
                | ImGuiTableFlags_NoHostExtendX | ImGuiTableFlags_Sortable;
            if (ImGui::BeginTable(boost::locale::translate("Entities").str().c_str(), 3, flags))
            {
                ImGui::TableSetupColumn(boost::locale::translate("Type").str().c_str(), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultSort);
                ImGui::TableSetupColumn(boost::locale::translate("UUID").str().c_str(), ImGuiTableColumnFlags_WidthFixed | ImGuiTableColumnFlags_DefaultSort);
                ImGui::TableSetupColumn(boost::locale::translate("Name").str().c_str(), ImGuiTableColumnFlags_WidthStretch | ImGuiTableColumnFlags_DefaultSort);
                ImGui::TableSetupScrollFreeze(0, 1);
                ImGui::TableHeadersRow();

                if (ImGuiTableSortSpecs* sortSpecs = ImGui::TableGetSortSpecs())
                {
                    if (sortSpecs->SpecsDirty)
                    {
                        const auto tableSortSpec = sortSpecs->Specs;
                        const auto direction = tableSortSpec->SortDirection;
                        textModel.Sort(direction == ImGuiSortDirection_Ascending, static_cast<Storyteller::TextObjectModel::Sorter::SortValue>(tableSortSpec->ColumnIndex));
                        sortSpecs->SpecsDirty = false;
                    }
                }

                auto texts = textModel.GetObjects();
                for (int row = 0; row < texts.size(); row++)
                {
                    const auto text = texts[row];
                    auto selected = textModel.IsSelected(text->GetUuid());
                    ImGui::TableNextRow();
                    ImGui::TableNextColumn();
                    ImGui::Selectable(text->GetTypename().c_str(), &selected, ImGuiSelectableFlags_SpanAllColumns);
                    if (ImGui::IsItemClicked(0))
                    {
                        textModel.Select(text->GetUuid());
                    }
                    
                    ImGui::TableNextColumn();
                    ImGui::Text(std::to_string(text->GetUuid()).c_str());
                    ImGui::TableNextColumn();
                    ImGui::Text(text->GetName().c_str());
                }

                ImGui::EndTable();
            }
            ImGui::End();
        }

        {
            ImGui::Begin(boost::locale::translate("Texts").str().c_str());
            ImGui::Text(boost::locale::translate("Name").str().c_str());
            const auto selectedEntity = textModel.GetSelected();
            const auto selectedUuid = selectedEntity ? selectedEntity->GetUuid() : Storyteller::UUID(0);
            auto entityName = selectedEntity ? selectedEntity->GetName() : std::string();
            if (ImGui::InputText(std::string("##EntityName").append(std::to_string(selectedUuid)).c_str(), &entityName) && selectedEntity)
            {
                selectedEntity->SetName(entityName);
            }

            ImGui::Text(boost::locale::translate("Source text").str().c_str());
            auto sourceText = selectedEntity ? selectedEntity->GetText() : std::string();
            if (ImGui::InputTextMultiline("##EntityText", &sourceText, ImVec2(-FLT_MIN, ImGui::GetContentRegionAvail().y / 2.0)) && selectedEntity)
            {
                selectedEntity->SetText(sourceText);
            }

            ImGui::Text(boost::locale::translate("Translation").str().c_str());
            ImGui::InputTextMultiline("##Translation", &sourceText, ImVec2(-FLT_MIN, ImGui::GetContentRegionAvail().y), ImGuiInputTextFlags_ReadOnly);

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