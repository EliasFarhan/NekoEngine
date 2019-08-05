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

    ImGui::Begin("Entity Viewer", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoCollapse);

    bool sceneOpen = ImGui::CollapsingHeader("Scene", ImGuiTreeNodeFlags_DefaultOpen);
    if (ImGui::BeginDragDropTarget())
    {
        ImGuiDragDropFlags target_flags = 0;
        //target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery;    // Don't wait until the delivery (release mouse button on a target) to do something
        target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect; // Don't display the yellow rectangle
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_NAME", target_flags))
        {
            neko::Entity move_from = *(const neko::Entity*) payload->Data;
            neko::Entity move_to = neko::INVALID_ENTITY;
            transformManager.SetTransformParent(move_from, move_to);
        }
        ImGui::EndDragDropTarget();
    }

    for (neko::Entity entity = 0; entity < entityManager.GetEntitiesSize(); entity++)
    {
        if (entityManager.EntityExists(entity))
        {
            entities_.push_back(entity);
            entitiesName_.push_back(sceneManager.GetCurrentScene().entitiesNames[entity]);
        }
    }
    std::set<neko::Entity> entitiesSet;

    for (size_t i = 0; i < entities_.size(); i++)
    {

        if(transformManager.GetParentEntity(entities_[i]) == neko::INVALID_ENTITY and
        std::find(entitiesSet.cbegin(), entitiesSet.cend(), entities_[i]) == entitiesSet.end())
        {
            DrawEntityHierarchy(entities_[i], i, transformManager, entityManager, entitiesSet, sceneOpen, false);
        }
    }

    if (ImGui::Button("Add Entity"))
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
                                       neko::EntityManager& entityManager, std::set<neko::Entity>& entitySet, bool draw,
                                       bool destroy)
{
    bool nodeOpen = draw;
    bool destroyEntity = destroy;
    bool leaf = transformManager.FindNextChild(entity, neko::INVALID_ENTITY) == neko::INVALID_ENTITY;

    if(draw)
    {
        ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_DefaultOpen;
        if(!leaf)
        {
            nodeFlags |= ImGuiTreeNodeFlags_OpenOnArrow;
        }
        else
        {
            nodeFlags |= ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen;
        }
        if (entity == selectedEntity_)
        {
            nodeFlags |= ImGuiTreeNodeFlags_Selected;
        }
        ImGui::PushItemWidth(ImGui::GetContentRegionAvail().x * 0.7f);
        nodeOpen = ImGui::TreeNodeEx((void*) (intptr_t) entities_[index], nodeFlags, "%s",
                                     entitiesName_[index].c_str());
        ImGui::PopItemWidth();
        if (ImGui::IsItemClicked())
            selectedEntity_ = entity;
        ImGuiDragDropFlags srcFlags = 0;
        srcFlags |= ImGuiDragDropFlags_SourceNoDisableHover;     // Keep the source displayed as hovered
        srcFlags |= ImGuiDragDropFlags_SourceNoHoldToOpenOthers; // Because our dragging is local, we disable the feature of opening foreign treenodes/tabs while dragging

        if (ImGui::BeginDragDropSource(srcFlags))
        {
            if (!(srcFlags & ImGuiDragDropFlags_SourceNoPreviewTooltip))
            {
                ImGui::Text("Moving Entity \"%s\"", entitiesName_[index].c_str());
            }
            ImGui::SetDragDropPayload("DND_DEMO_NAME", &entities_[index], sizeof(neko::Entity));
            ImGui::EndDragDropSource();
        }
        if (ImGui::BeginDragDropTarget())
        {
            ImGuiDragDropFlags targetFlags = 0;
            //target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery;    // Don't wait until the delivery (release mouse button on a target) to do something
            targetFlags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect; // Don't display the yellow rectangle
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_NAME", targetFlags))
            {
                neko::Entity moveFrom = *(const neko::Entity*) payload->Data;
                neko::Entity moveTo = entities_[index];
                transformManager.SetTransformParent(moveFrom, moveTo);
            }
            ImGui::EndDragDropTarget();
        }
        ImGui::SameLine();
        std::string buttonId = "Remove Entity "+std::to_string(entity);
        ImGui::PushID(buttonId.c_str());
        if(ImGui::Button("Remove"))
        {
            destroyEntity = true;
        }
        ImGui::PopID();
    }
    entitySet.emplace(entity);
    if(destroyEntity)
    {
        entityManager.DestroyEntity(entity);
    }
    auto entityChild = neko::INVALID_ENTITY;
    do
    {
        entityChild = transformManager.FindNextChild(entity, entityChild);
        if(entityChild != neko::INVALID_ENTITY)
        {
            size_t childIndex = std::find(entities_.cbegin(), entities_.cend(), entityChild) - entities_.begin();
            DrawEntityHierarchy(entityChild, childIndex, transformManager, entityManager, entitySet, nodeOpen, destroyEntity);
        }
    }
    while (entityChild != neko::INVALID_ENTITY);
    if(nodeOpen and !leaf)
        ImGui::TreePop();
}
}