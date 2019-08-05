//
// Created by efarhan on 7/31/19.
//

#include <tools/entity_viewer.h>
#include <imgui.h>
#include <tools/editor_scene.h>
#include <utilities/vector_utility.h>
#include <engine/transform.h>
#include <engine/log.h>

namespace editor
{
void EntityViewer::Update(neko::EntityManager& entityManager,
                          EditorSceneManager& sceneManager,
                          neko::Transform2dManager& transformManager)
{
    entities_.clear();
    entitiesName_.clear();
    for (neko::Entity entity = 0; entity < entityManager.GetEntitiesSize(); entity++)
    {
        if (entityManager.EntityExists(entity))
        {
            entities_.push_back(entity);
            entitiesName_.push_back(sceneManager.GetCurrentScene().entitiesNames[entity]);
        }
    }
    std::set<neko::Entity> entitiesSet;
    ImGui::Begin("Entity Viewer", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);
    for (size_t i = 0; i < entities_.size(); i++)
    {
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.7f);

/*
        if (ImGui::Selectable(entitiesName_[i].c_str(), selectedEntity_ == entities_[i], ImGuiSelectableFlags_None,
                              ImVec2(ImGui::GetContentRegionAvail().x * 0.7f, 0)))
        {
            selectedEntity_ = entities_[i];
        }



        ImGui::PopItemWidth();
        ImGui::SameLine();
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.3f);
        std::string removeLabel = "Remove Entity " + std::to_string(entities_[i]);
        ImGui::PushID(removeLabel.c_str());
        if (ImGui::Button("Remove"))
        {
            entityManager.DestroyEntity(entities_[i]);
            if (selectedEntity_ == entities_[i])
                selectedEntity_ = neko::INVALID_ENTITY;
        }
        ImGui::PopID();
        ImGui::PopItemWidth();
        */
        if(transformManager.GetParentEntity(entities_[i]) == neko::INVALID_ENTITY and
        std::find(entitiesSet.cbegin(), entitiesSet.cend(), entities_[i]) == entitiesSet.end())
        {
            DrawEntityHierarchy(entities_[i], i, transformManager, entitiesSet, true);
        }
    }
    if (ImGui::Button("Add"))
    {
        auto entity = entityManager.CreateEntity();
        auto& entitiesName = sceneManager.GetCurrentScene().entitiesNames;
        ResizeIfNecessary(entitiesName, entity, std::string());
        entitiesName[entity] = std::string("Entity ") + std::to_string(entity);
        transformManager.SetTransformParent(entity, neko::INVALID_ENTITY);
    }

    ImGui::End();
}

void EntityViewer::DrawEntityHierarchy(neko::Entity entity, size_t index, neko::Transform2dManager& transformManager,
                                       std::set<neko::Entity>& entitySet, bool draw)
{
    bool nodeOpen = draw;
    if(draw)
    {
        ImGuiTreeNodeFlags node_flags = ImGuiTreeNodeFlags_OpenOnArrow;
        if (entity == selectedEntity_)
        {
            node_flags |= ImGuiTreeNodeFlags_Selected;
        }
        nodeOpen = ImGui::TreeNodeEx((void*) (intptr_t) entities_[index], node_flags, "%s",
                                          entitiesName_[index].c_str());
        if (ImGui::IsItemClicked())
            selectedEntity_ = entity;
        ImGuiDragDropFlags src_flags = 0;
        src_flags |= ImGuiDragDropFlags_SourceNoDisableHover;     // Keep the source displayed as hovered
        src_flags |= ImGuiDragDropFlags_SourceNoHoldToOpenOthers; // Because our dragging is local, we disable the feature of opening foreign treenodes/tabs while dragging

        bool canParentTransform = false;
        if (ImGui::BeginDragDropSource(src_flags))
        {
            if (!(src_flags & ImGuiDragDropFlags_SourceNoPreviewTooltip))
            {
                ImGui::Text("Moving Entity \"%s\"", entitiesName_[index].c_str());
            }
            ImGui::SetDragDropPayload("DND_DEMO_NAME", &entities_[index], sizeof(neko::Entity));
            ImGui::EndDragDropSource();
        }
        if (ImGui::BeginDragDropTarget())
        {
            ImGuiDragDropFlags target_flags = 0;
            //target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery;    // Don't wait until the delivery (release mouse button on a target) to do something
            target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect; // Don't display the yellow rectangle
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_NAME", target_flags))
            {
                neko::Entity move_from = *(const neko::Entity*) payload->Data;
                neko::Entity move_to = entities_[index];
                transformManager.SetTransformParent(move_from, move_to);
            }
            ImGui::EndDragDropTarget();
        }
    }
    entitySet.emplace(entity);
    auto entityChild = neko::INVALID_ENTITY;
    do
    {
        entityChild = transformManager.FindNextChild(entity, entityChild);
        if(entityChild != neko::INVALID_ENTITY)
        {
            size_t childIndex = std::find(entities_.cbegin(), entities_.cend(), entityChild) - entities_.begin();
            DrawEntityHierarchy(entityChild, childIndex, transformManager, entitySet, nodeOpen);
        }
    }
    while (entityChild != neko::INVALID_ENTITY);
    if(nodeOpen)
        ImGui::TreePop();
}
}