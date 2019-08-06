//
// Created by efarhan on 7/31/19.
//

#include <tools/entity_viewer.h>
#include <imgui.h>
#include <tools/editor_scene.h>
#include <utilities/vector_utility.h>
#include <engine/transform.h>
#include <engine/log.h>
#include <tools/neko_editor.h>

namespace editor
{
void EntityViewer::Update()
{
    auto& sceneManager = nekoEditor_.GetSceneManager();
    auto& entityManager = nekoEditor_.GetEntityManager();
    auto& transformManager = nekoEditor_.GetTransformManager();
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
            DrawEntityHierarchy(entities_[i], i, entitiesSet, sceneOpen, false);
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

void EntityViewer::DrawEntityHierarchy(neko::Entity entity,
        size_t index,
        std::set<neko::Entity>& entitySet,
        bool draw,
        bool destroy)
{
    auto& transformManager = nekoEditor_.GetTransformManager();
    bool nodeOpen = draw;
    bool destroyEntity = destroy;
    bool createEntity = false;
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

        const std::string entityPopupName = "Entity Popup "+std::to_string(entity);
        if(ImGui::IsItemClicked(1))
        {
            logDebug("Left Clicked on Entity: "+std::to_string(entity));

            ImGui::OpenPopup(entityPopupName.c_str());
        }

        if(ImGui::BeginPopup(entityPopupName.c_str()))
        {
            const std::string entityMenuName = "Entity Menu "+std::to_string(entity);
            enum class EntityMenuComboItem
            {
                ADD_EMPTY_ENTITY,
                DELETE,
                LENGTH
            };
            const char* entityMenuComboItemName[int(EntityMenuComboItem::LENGTH)] = {
                    "Add Empty Entity",
                    "Delete Entity"
            };

            const auto entityComboName = entityMenuName + " Combo";
            for(int i = 0 ; i < int(EntityMenuComboItem::LENGTH);i++)
            {
                const auto key = entityComboName+" "+entityMenuComboItemName[i];
                ImGui::PushID(key.c_str());
                if(ImGui::Selectable(entityMenuComboItemName[i]))
                {
                    const auto item = EntityMenuComboItem(i);
                    switch (item)
                    {
                        case EntityMenuComboItem::ADD_EMPTY_ENTITY:
                        {
                            createEntity = true;
                            break;
                        }
                        case EntityMenuComboItem::DELETE:
                        {
                            destroyEntity = true;
                            break;
                        }
                        default:
                            break;
                    }
                    ImGui::CloseCurrentPopup();
                }
                ImGui::PopID();
            }

            ImGui::EndPopup();
        }
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
    }
    entitySet.emplace(entity);
    if(destroyEntity)
    {
        auto& entityManager = nekoEditor_.GetEntityManager();
        entityManager.DestroyEntity(entity);
    }
    auto entityChild = neko::INVALID_ENTITY;
    do
    {
        auto& entityManager = nekoEditor_.GetEntityManager();
        entityChild = transformManager.FindNextChild(entity, entityChild);
        if(entityChild != neko::INVALID_ENTITY and entityManager.EntityExists(entityChild))
        {
            size_t childIndex = std::find(entities_.cbegin(), entities_.cend(), entityChild) - entities_.begin();
            DrawEntityHierarchy(entityChild, childIndex, entitySet, nodeOpen, destroyEntity);
        }
    }
    while (entityChild != neko::INVALID_ENTITY);

    if(createEntity)
    {
        auto& sceneManager = nekoEditor_.GetSceneManager();
        auto& entityManager = nekoEditor_.GetEntityManager();

        auto newEntity = entityManager.CreateEntity();
        auto& entitiesName = sceneManager.GetCurrentScene().entitiesNames;
        ResizeIfNecessary(entitiesName, newEntity, std::string());
        entitiesName[newEntity] = std::string("Entity ") + std::to_string(newEntity);
        transformManager.SetTransformParent(newEntity, entity);
    }
    if(nodeOpen and !leaf)
        ImGui::TreePop();
}

EntityViewer::EntityViewer(NekoEditor& editor) : nekoEditor_(editor)
{

}
}