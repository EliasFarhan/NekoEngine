//
// Created by efarhan on 7/31/19.
//

#include <tools/entity_viewer.h>
#include <imgui.h>
#include <tools/editor_scene.h>
#include <utilities/vector_utility.h>

namespace editor
{
void EntityViewer::Update(neko::EntityManager& entityManager, EditorSceneManager& sceneManager)
{
    entities_.clear();
    entitiesName_.clear();
    for(neko::Entity entity = 0; entity < entityManager.GetEntitiesSize(); entity++)
    {
        if(entityManager.EntityExists(entity))
        {
            entities_.push_back(entity);
            entitiesName_.push_back(sceneManager.GetCurrentScene().entitiesNames[entity]);
        }
    }
    ImGui::Begin("Entity Viewer", nullptr, ImGuiWindowFlags_NoResize|ImGuiWindowFlags_NoCollapse);
    for(int i = 0; i < entities_.size();i++)
    {
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.7f);
        if(ImGui::Selectable(entitiesName_[i].c_str(), selectedEntity_ == entities_[i], ImGuiSelectableFlags_None,
                ImVec2(ImGui::GetContentRegionAvail().x * 0.7f, 0)))
        {
            selectedEntity_ = entities_[i];
        }
        ImGui::PopItemWidth();
        ImGui::SameLine();
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.3f);
        std::string removeLabel = "Remove Entity "+std::to_string(entities_[i]);
        ImGui::PushID(removeLabel.c_str());
        if(ImGui::Button("Remove"))
        {
            entityManager.DestroyEntity(entities_[i]);
            if(selectedEntity_ == entities_[i])
                selectedEntity_ = neko::INVALID_ENTITY;
        }
        ImGui::PopID();
        ImGui::PopItemWidth();
    }
    if(ImGui::Button("Add"))
    {
        auto entity = entityManager.CreateEntity();
        auto& entitiesName = sceneManager.GetCurrentScene().entitiesNames;
        ResizeIfNecessary(entitiesName, entity, std::string());
        entitiesName[entity] = std::string("Entity ")+std::to_string(entity);
    }
    ImGui::End();
}
}