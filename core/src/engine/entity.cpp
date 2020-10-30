/*
 MIT License

 Copyright (c) 2020 SAE Institute Switzerland AG

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */
#include <engine/entity.h>
#include "engine/globals.h"
#include <algorithm>
#include <utilities/vector_utility.h>
#include <engine/component.h>
#include <sstream>
#include <engine/log.h>

#include <fmt/format.h>
#include "imgui.h"

namespace neko
{

EntityManager::EntityManager()
{
    entityMaskArray_.resize(INIT_ENTITY_NMB);
	entityHashArray_.resize(INIT_ENTITY_NMB);
    parentEntities_.resize(INIT_ENTITY_NMB, INVALID_ENTITY);
}

EntityMask EntityManager::GetMask(Entity entity)
{
    return entityMaskArray_[entity];
}

Entity EntityManager::CreateEntity(Entity entity)
{
    if(entity == INVALID_ENTITY)
    {
        const auto entityMaskIt = std::find_if(entityMaskArray_.begin(), entityMaskArray_.end(),
                                               [](EntityMask entityMask)
                                               {
                                                   return entityMask == INVALID_ENTITY_MASK;
                                               });
        if (entityMaskIt == entityMaskArray_.end())
        {
            const auto newEntity = entityMaskArray_.size();
            ResizeIfNecessary(entityMaskArray_, newEntity, INVALID_ENTITY_MASK);
            ResizeIfNecessary(parentEntities_, newEntity, INVALID_ENTITY);
			ResizeIfNecessary(entityHashArray_, newEntity, INVALID_ENTITY_HASH);
        	AddComponentType(Entity(newEntity), static_cast<EntityMask>(ComponentType::EMPTY));
            return Entity(newEntity);
        }
        else
        {
            const auto newEntity = entityMaskIt - entityMaskArray_.begin();
            AddComponentType(Entity(newEntity), static_cast<EntityMask>(ComponentType::EMPTY));
            return Entity(newEntity);
        }
    }
    else
    {
        ResizeIfNecessary(entityMaskArray_, entity, INVALID_ENTITY_MASK);
        ResizeIfNecessary(parentEntities_, entity, INVALID_ENTITY);
		ResizeIfNecessary(entityHashArray_, entity, INVALID_ENTITY_HASH);
    	if(!EntityExists(entity))
        {
            AddComponentType(entity, static_cast<EntityMask>(ComponentType::EMPTY));
            return entity;
        }
        return CreateEntity(INVALID_ENTITY);
    }
}

void EntityManager::DestroyEntity(Entity entity)
{
    entityMaskArray_[entity] = INVALID_ENTITY_MASK;
	entityHashArray_[entity] = INVALID_ENTITY_HASH;

	onDestroyEntity.Execute(entity);
}

bool EntityManager::HasComponent(Entity entity, EntityMask componentType) const
{
	if (entity >= entityMaskArray_.size())
    {
	    std::ostringstream oss;
	    oss << "[Error] Accessing entity: "<<entity<<" while entity mask array is of size: "<<entityMaskArray_.size();
	    logDebug(oss.str());
	    return false;
    }
    return (entityMaskArray_[entity] & EntityMask(componentType)) == EntityMask(componentType);
}

void EntityManager::AddComponentType(Entity entity, EntityMask componentType)
{
    entityMaskArray_[entity] |= EntityMask(componentType);
}

void EntityManager::RemoveComponentType(Entity entity, EntityMask componentType)
{
    entityMaskArray_[entity] &= ~EntityMask(componentType);
}

void EntityManager::SetEntityName(Entity entity, const std::string& entityName)
{
	const auto entityHash = HashEntityName(entityName);
	entityHashArray_[entity] = entityHash;
}

Entity EntityManager::FindEntityByName(const std::string& entityName)
{
	const auto entityHash = HashEntityName(entityName);
	return FindEntityByHash(entityHash);
}

EntityHash EntityManager::HashEntityName(const std::string& entityName)
{
	xxh::hash_state_t<64> hash_stream(0);
	hash_stream.update(entityName);
	const EntityHash entityHash = hash_stream.digest();
	return entityHash;
}

DirtyManager::DirtyManager(EntityManager& entityManager) : entityManager_(entityManager)
{
}

void DirtyManager::SetDirty(Entity entity)
{
    if (std::find(dirtyEntities_.cbegin(), dirtyEntities_.cend(), entity) == dirtyEntities_.cend())
    {
    	dirtyEntities_.push_back(entity);
    }
}

void DirtyManager::UpdateDirtyEntities()
{
    //Fill the dirty entities with all the children in O(n)
    for (Entity entity = 0; entity < entityManager_.get().GetEntitiesSize(); entity++)
    {
    	if(!entityManager_.get().EntityExists(entity))
            continue;
        auto parent = entityManager_.get().GetEntityParent(entity);
        while (parent != INVALID_ENTITY)
        {
            if (std::find(dirtyEntities_.cbegin(), dirtyEntities_.cend(), parent) != dirtyEntities_.end())
            {
                SetDirty(entity);
                break;
            }
            parent = entityManager_.get().GetEntityParent(parent);
        }
    }
    for (auto entity : dirtyEntities_)
    {
        updateDirtyEntity.Execute(entity);
    }
    dirtyEntities_.clear();
}

EntityHierarchy::EntityHierarchy(EntityManager& entityManager) : entityManager_(entityManager)
{
    entityManager_.RegisterOnChangeParent(this);
}

void EntityHierarchy::OnChangeParent(Entity entity, Entity newParent, Entity oldParent)
{
	//Remove entity from previous parent
    if(oldParent != INVALID_ENTITY)
    {
        auto it = entityHierarchyMap_.find(oldParent);
    	if(it != entityHierarchyMap_.end())
    	{
            auto& hierarchy = it->second;
            const auto entityIt = std::find(hierarchy.begin(), hierarchy.end(), entity);
    		if(entityIt != hierarchy.end())
    		{
                hierarchy.erase(entityIt);
    		}
    	}
    }
	//Add entity to new parent
	if(newParent != INVALID_ENTITY)
	{
        auto it = entityHierarchyMap_.find(newParent);
		if(it != entityHierarchyMap_.end())
		{
            auto& hierarchy = it->second;
            hierarchy.push_back(entity);
		}
        else
        {
            entityHierarchyMap_[newParent] = { entity };
        }
	}
}

const std::vector<Entity>& EntityHierarchy::GetChildren(Entity entity)
{
    return entityHierarchyMap_[entity];
}

bool EntityHierarchy::HasChildren(Entity entity)
{
	const auto it = entityHierarchyMap_.find(entity);
	if(it != entityHierarchyMap_.end())
	{
        return !(it->second.empty());
	}
    return false;
}

EntityViewer::EntityViewer(EntityManager& entityManager, EntityHierarchy& entityHierarchy) :
	entityHierarchy_(entityHierarchy),
	entityManager_(entityManager)
{
}

void EntityViewer::DrawImGui()
{
    ImGui::Begin("Entities");

    if (ImGui::BeginDragDropTarget())
    {
        ImGuiDragDropFlags target_flags = 0;
        //target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery;    // Don't wait until the delivery (release mouse button on a target) to do something
        target_flags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect; // Don't display the yellow rectangle
        if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_NAME", target_flags))
        {
            const neko::Entity moveFrom = *static_cast<const neko::Entity*>(payload->Data);
            const neko::Entity moveTo = neko::INVALID_ENTITY;
            entityManager_.SetEntityParent(moveFrom, moveTo);
        }
        ImGui::EndDragDropTarget();
    }
	
	for(Entity entity = 0; entity < entityManager_.GetEntitiesSize(); entity++)
	{
		if(entityManager_.EntityExists(entity) and entityManager_.GetEntityParent(entity) == INVALID_ENTITY)
		{
            DrawEntityHierarchy(entity, true, false);
		}
	}
    if (ImGui::Button("Add Entity"))
    {
        [[maybe_unused]]const auto entity = entityManager_.CreateEntity();
    }
    ImGui::End();
}

void EntityViewer::DrawEntityHierarchy(neko::Entity entity, bool draw, bool destroy)
{
    const std::string entityName = "Entity " + std::to_string(entity + 1);
    bool nodeOpen = draw;
    bool destroyEntity = destroy;
    bool createEntity = false;

    const bool leaf = !entityHierarchy_.HasChildren(entity);

    if (draw)
    {
        ImGuiTreeNodeFlags nodeFlags = ImGuiTreeNodeFlags_DefaultOpen;
        if (!leaf)
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
        nodeOpen = ImGui::TreeNodeEx((void*)(intptr_t)entity, nodeFlags, "%s",
            entityName.c_str());
        ImGui::PopItemWidth();
        if (ImGui::IsItemClicked())
            selectedEntity_ = entity;
        const std::string entityPopupName = "Entity Popup " + std::to_string(entity);
        if (ImGui::IsItemClicked(1))
        {
            logDebug(fmt::format("Left Clicked on Entity: {}", entity));

            ImGui::OpenPopup(entityPopupName.c_str());
        }

        if (ImGui::BeginPopup(entityPopupName.c_str()))
        {
            const std::string entityMenuName = "Entity Menu " + std::to_string(entity);
            enum class EntityMenuComboItem
            {
                ADD_EMPTY_ENTITY,
                DELETE_ENTITY,
                LENGTH
            };
            const char* entityMenuComboItemName[int(EntityMenuComboItem::LENGTH)] = {
                    "Add Empty Entity",
                    "Delete Entity"
            };

            const auto entityComboName = entityMenuName + " Combo";
            for (int i = 0; i < int(EntityMenuComboItem::LENGTH); i++)
            {
                const auto key = entityComboName + " " + entityMenuComboItemName[i];
                ImGui::PushID(key.c_str());
                if (ImGui::Selectable(entityMenuComboItemName[i]))
                {
                    const auto item = EntityMenuComboItem(i);
                    switch (item)
                    {
                    case EntityMenuComboItem::ADD_EMPTY_ENTITY:
                    {
                        createEntity = true;
                        break;
                    }
                    case EntityMenuComboItem::DELETE_ENTITY:
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
                ImGui::Text("Moving Entity \"%s\"", entityName.c_str());
            }
            ImGui::SetDragDropPayload("DND_DEMO_NAME", &entity, sizeof(neko::Entity));
            ImGui::EndDragDropSource();
        }
        if (ImGui::BeginDragDropTarget())
        {
            ImGuiDragDropFlags targetFlags = 0;
            //target_flags |= ImGuiDragDropFlags_AcceptBeforeDelivery;    // Don't wait until the delivery (release mouse button on a target) to do something
            targetFlags |= ImGuiDragDropFlags_AcceptNoDrawDefaultRect; // Don't display the yellow rectangle
            if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("DND_DEMO_NAME", targetFlags))
            {
	            const neko::Entity moveFrom = *(const neko::Entity*) payload->Data;
	            const neko::Entity moveTo = entity;
                entityManager_.SetEntityParent(moveFrom, moveTo);
            }
            ImGui::EndDragDropTarget();
        }
    }

    if (!leaf)
    {
        const auto& children = entityHierarchy_.GetChildren(entity);
        for (auto entityChild : children)
        {
            if (entityManager_.EntityExists(entityChild))
            {
                DrawEntityHierarchy(entityChild, nodeOpen, destroyEntity);
            }
        }
    }
    if (createEntity)
    {
	    const auto newEntity = entityManager_.CreateEntity();
        entityManager_.SetEntityParent(newEntity, entity);
    }
    if (destroyEntity)
    {
        entityManager_.DestroyEntity(entity);
    }
    if (nodeOpen and !leaf)
    {
        ImGui::TreePop();
    }
}

size_t EntityManager::GetEntitiesNmb(EntityMask filterComponents)
{
    return std::count_if(entityMaskArray_.begin(), entityMaskArray_.end(),[&filterComponents](EntityMask entityMask){
        return entityMask != INVALID_ENTITY_MASK && (entityMask & EntityMask(filterComponents)) == EntityMask(filterComponents);
    });
}

size_t EntityManager::GetEntitiesSize() const
{
	return entityMaskArray_.size();
}

std::vector<Entity> EntityManager::FilterEntities(EntityMask filterComponents) const
{
	std::vector<Entity> entities;
	entities.reserve(entityMaskArray_.size());
	for(Entity i = 0; i < entityMaskArray_.size();i++)
	{
		if(HasComponent(i, filterComponents))
		{
			entities.push_back(i);
		}
	}
	return entities;
}

bool EntityManager::EntityExists(Entity entity)
{
    return entityMaskArray_[entity] != INVALID_ENTITY_MASK;
}

Entity EntityManager::GetLastEntity()
{
    const auto it = std::find_if(
            entityMaskArray_.rbegin(),
            entityMaskArray_.rend(),
                    [](EntityMask entityMask){
       return entityMask != INVALID_ENTITY_MASK;
    });

    return Entity(std::distance(entityMaskArray_.begin(), it.base()) - 1);
}

bool EntityManager::IsPrefab(Entity entity) const
{
    return HasComponent(entity, static_cast<EntityMask>(ComponentType::PREFAB));
}

EntityHash EntityManager::GetEntityNameHash(Entity entity)
{
    return entityHashArray_[entity];
}

Entity EntityManager::FindEntityByHash(EntityHash entityHash)
{
	const auto index = std::find(entityHashArray_.begin(), entityHashArray_.end(), entityHash);
	if (index != entityHashArray_.end())
	{
		return index - entityHashArray_.begin();
	}
	return INVALID_ENTITY;
}

void EntityManager::SetEntityNameHash(Entity entity, EntityHash entityHash)
{
    entityHashArray_[entity] = entityHash;
}

void EntityManager::RegisterOnChangeParent(OnChangeParentInterface* onChangeInterface)
{
    onChangeParent.RegisterCallback([onChangeInterface](Entity entity, Entity newParent, Entity oldParent)
    {
        onChangeInterface->OnChangeParent(entity, newParent, oldParent);
    });
}

Entity EntityManager::GetEntityParent(Entity entity)
{
    return parentEntities_[entity];
}

bool EntityManager::SetEntityParent(Entity child, Entity parent)
{
	const auto oldParent = GetEntityParent(child);
    auto p = GetEntityParent(parent);
    while (p != INVALID_ENTITY)
    {
	    if(p == child)
	    {
            logDebug(fmt::format("[Warning] Child entity: {} cannot have parent entity: {}", child , parent));
            return false;
	    }
        p = GetEntityParent(p);
    }
    parentEntities_[child] = parent;
    onChangeParent.Execute(child, parent, oldParent);
    return true;
}
}
