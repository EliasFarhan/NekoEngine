//
// Created by efarhan on 01/01/2021.
//

#include <editor_hierarchy.h>
#include <imgui.h>
#include <engine/log.h>

namespace neko::editor
{

void EditorHierarchy::Init()
{

}

void EditorHierarchy::Update(seconds dt)
{

}

void EditorHierarchy::Destroy()
{

}

void EditorHierarchy::CreateEmptyEntity(Entity parent) const
{
    auto lastEntity = parent == INVALID_ENTITY ? entityManager_.GetFirstRoot() : entityDataManager_.GetComponent(parent).firstChild;
    // We are jumping siblings until we have the true last root entity
    while (lastEntity != INVALID_ENTITY)
    {
        const auto& lastEntityData = entityDataManager_.GetComponent(lastEntity);
        if (lastEntityData.nextSibling == INVALID_ENTITY)
        {
            break;
        }
        lastEntity = lastEntityData.nextSibling;
    }
    const auto newEntity = entityManager_.CreateEntity();
    const auto newEntityName = fmt::format("Entity {}", newEntity);
    entityManager_.SetEntityName(newEntity, newEntityName);
    entityDataManager_.AddComponent(newEntity);
    auto newEntityData = entityDataManager_.GetComponent(newEntity);
    newEntityData.name = newEntityName;
    entityDataManager_.SetComponent(newEntity, newEntityData);
    // We update the last root entity to point to the new created entity
    if (lastEntity != INVALID_ENTITY)
    {
        auto lastEntityData = entityDataManager_.GetComponent(lastEntity);
        lastEntityData.nextSibling = newEntity;
        entityDataManager_.SetComponent(lastEntity, lastEntityData);
    }
    // We update the parent as its child
    if(parent != INVALID_ENTITY)
    {
        entityManager_.SetEntityParent(newEntity, parent);
        const auto& parentEntityData = entityDataManager_.GetComponent(parent);
        if(parentEntityData.firstChild == INVALID_ENTITY)
        {
            auto newParentEntityData = parentEntityData;
            newParentEntityData.firstChild = newEntity;
            entityDataManager_.SetComponent(parent, newParentEntityData);
        }
    }
}

void EditorHierarchy::SetParentEntity(Entity child, Entity parent) const
{
    // Checking that child is not a parent of parent
    auto tmpParent = entityManager_.GetEntityParent(parent);
    while (tmpParent != INVALID_ENTITY)
    {
        if(tmpParent == child)
            return;
        tmpParent = entityManager_.GetEntityParent(tmpParent);
    }
    logDebug(fmt::format("Adding child entity {} to parent entity {}", child, parent));

    const auto currentChildParent = entityManager_.GetEntityParent(child);
    entityManager_.SetEntityParent(child, parent);
    auto childData = entityDataManager_.GetComponent(child);

    Entity previousChildSibling;
    if(currentChildParent == INVALID_ENTITY)
    {
        // Search the siblings into the roots
        previousChildSibling = entityManager_.GetFirstRoot();
    }
    else
    {
        previousChildSibling = entityDataManager_.GetComponent(currentChildParent).firstChild;

        if (previousChildSibling == child)
        {
            auto currentParentData = entityDataManager_.GetComponent(currentChildParent);
            currentParentData.firstChild = childData.nextSibling;
            entityDataManager_.SetComponent(currentChildParent, currentParentData);
        }
    }
    if(previousChildSibling == child)
    {
        previousChildSibling = INVALID_ENTITY;
    }
    while(previousChildSibling != INVALID_ENTITY)
    {
        const auto& previousSiblingData = entityDataManager_.GetComponent(previousChildSibling);
        if(previousSiblingData.nextSibling == child)
        {
            auto newPreviousChildData = previousSiblingData;
            newPreviousChildData.nextSibling = childData.nextSibling != previousChildSibling? childData.nextSibling : INVALID_ENTITY;
            entityDataManager_.SetComponent(previousChildSibling, newPreviousChildData);
            break;
        }
        previousChildSibling = previousSiblingData.nextSibling;
    }
    childData.nextSibling = INVALID_ENTITY;
    entityDataManager_.SetComponent(child, childData);
    // Add the child into the parent child
    auto parentData = entityDataManager_.GetComponent(parent);
    if(parentData.firstChild == INVALID_ENTITY)
    {
        parentData.firstChild = child;
        entityDataManager_.SetComponent(parent, parentData);
    }
    else
    {
        auto lastChild = parentData.firstChild;
        while (true)
        {
            const auto& lastChildData = entityDataManager_.GetComponent(lastChild);
            if(lastChildData.nextSibling == INVALID_ENTITY)
            {
                auto newLastChildData = lastChildData;
                newLastChildData.nextSibling = child;
                entityDataManager_.SetComponent(lastChild, newLastChildData);
                break;
            }
            lastChild = lastChildData.nextSibling;
        }
    }

}

void EditorHierarchy::DrawImGui()
{
    ImGui::Begin("Entity Hierarchy");
    bool isRightClickOnItem = false;
    std::function<void(Entity)> DrawEntityTree = [this, &DrawEntityTree, &isRightClickOnItem](Entity entity)
    {
        if (entity == INVALID_ENTITY)
            return;
        const auto& entityData = entityDataManager_.GetComponent(entity);
        const auto treeId = fmt::format("Entity {}", entity);
        ImGuiTreeNodeFlags flags = ImGuiTreeNodeFlags_OpenOnArrow | 
            ImGuiTreeNodeFlags_OpenOnDoubleClick | 
            ImGuiTreeNodeFlags_SpanAvailWidth |
            ImGuiTreeNodeFlags_DefaultOpen;
        if(selectedEntity_ == entity)
        {
            flags |= ImGuiTreeNodeFlags_Selected;
        }
        bool isLeaf = false;
        if(entityData.firstChild == INVALID_ENTITY)
        {
            flags |= ImGuiTreeNodeFlags_Leaf;
            isLeaf = true;
        }
        const bool treeOpen = ImGui::TreeNodeEx(reinterpret_cast<void*>(static_cast<intptr_t>(entity)), flags, "%s", entityData.name.c_str());
        // Select entity if clicked
        if (ImGui::IsItemClicked(0))
        {
            selectedEntity_ = entity;
        }
        // Open menu popup
        if(ImGui::BeginPopupContextItem("Entity Popup"))
        {
            if(ImGui::Selectable("Create Child Empty Entity"))
            {
                CreateEmptyEntity(entity);
            }
            ImGui::EndPopup();
        }
        if (ImGui::IsItemClicked(1))
        {
            isRightClickOnItem = true;
            ImGui::OpenPopup("Hierarchy Popup");
        }
        if(ImGui::BeginDragDropSource())
        {
            ImGui::SetDragDropPayload("ENTITY", &entity, sizeof(Entity), ImGuiCond_Always);
            ImGui::Text("Drag %s", entityData.name.c_str());
            ImGui::EndDragDropSource();
        }
        bool justDropTarget = false;
        if(ImGui::BeginDragDropTarget())
        {
            const auto* data = ImGui::AcceptDragDropPayload("ENTITY");
            if (data != nullptr)
            {
                const Entity newChild = *static_cast<Entity*>(data->Data);
                SetParentEntity(newChild, entity);
                justDropTarget = isLeaf;
            }
            ImGui::EndDragDropTarget();
        }
        if(treeOpen && !justDropTarget && entityData.firstChild != INVALID_ENTITY)
        {
            DrawEntityTree(entityData.firstChild);

        }
        if(treeOpen)
        {
            ImGui::TreePop();
        }
       
        if(entityData.nextSibling != INVALID_ENTITY)
        {
            DrawEntityTree(entityData.nextSibling);
        }
    };
    DrawEntityTree(entityManager_.GetFirstRoot());

    if (!isRightClickOnItem && ImGui::GetIO().MouseClicked[1] && ImGui::IsWindowHovered())
    {
        ImGui::OpenPopup("Hierarchy Popup");
    }

    if(ImGui::BeginPopup("Hierarchy Popup"))
    {
        if(ImGui::Selectable("Create Empty Entity"))
        {
            CreateEmptyEntity(INVALID_ENTITY);
        }
        ImGui::EndPopup();
    }
    ImGui::End();
}

EditorHierarchy::EditorHierarchy(
        EntityManager& entityManager,
        EntityDataManager& entityNameManager) :
    entityManager_(entityManager),
    entityDataManager_(entityNameManager)
{

}
}