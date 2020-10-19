#pragma once
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

#include <unordered_map>
#include <vector>
#include <engine/globals.h>
#include <xxhash.hpp>
#include "utilities/action_utility.h"
#include "engine/system.h"

namespace neko
{
class OnChangeParentInterface;
/**
 * \brief Entity start at 0 and goes to N
 */
using Entity = Index;
/**
 * Use to find a specific entity
 */
using EntityHash = xxh::hash_t<64>;
/**
 * \brief EntityMask is a bitmask representation of the activated components
 */
using EntityMask = std::uint32_t;
const Entity INVALID_ENTITY = std::numeric_limits<Index>::max();
const EntityMask INVALID_ENTITY_MASK = 0u;
const EntityHash INVALID_ENTITY_HASH = EntityHash(0);
enum class ComponentType : std::uint32_t;

template<typename T, EntityMask componentType>
class ComponentManager;

/**
 * \brief Used in an Entity-Component-System to store all entities and what components they have
 */
class EntityManager 
{
public:
    explicit EntityManager();

    EntityMask GetMask(Entity entity);
    /**
     * \brief create an empty entity (non-null EntityMask)
     */
    Entity CreateEntity(Entity entity = INVALID_ENTITY);

    Entity GetLastEntity();

    void DestroyEntity(Entity entity);
    /**
     *
     */
    [[nodiscard]] bool HasComponent(Entity entity, EntityMask componentType) const;

    [[nodiscard]] bool IsPrefab(Entity entity) const;

    [[nodiscard]] bool EntityExists(Entity entity);

    [[nodiscard]] size_t GetEntitiesNmb(EntityMask filterComponents = INVALID_ENTITY_MASK);

    [[nodiscard]] size_t GetEntitiesSize() const;

    [[nodiscard]] std::vector<Entity> FilterEntities(EntityMask filterComponents = INVALID_ENTITY_MASK) const;

    void AddComponentType(Entity entity, EntityMask componentType);

    void RemoveComponentType(Entity entity, EntityMask componentType);

    void SetEntityName(Entity entity, const std::string& entityName);

    void SetEntityNameHash(Entity entity, EntityHash entityHash);

    [[nodiscard]] EntityHash GetEntityNameHash(Entity entity);

    [[nodiscard]] Entity FindEntityByHash(EntityHash entityHash);

    [[nodiscard]] Entity FindEntityByName(const std::string& entityName);

    template<typename T, EntityMask componentType>
    void RegisterComponentManager(ComponentManager<T, componentType>& componentManager)
    {
        onDestroyEntity.RegisterCallback(
                [&componentManager](Entity entity) { componentManager.DestroyComponent(entity); });
    }

    void RegisterOnChangeParent(OnChangeParentInterface* onChangeInterface);

    Entity GetEntityParent(Entity entity);

	/**
	 * \brief Set the entity parent and check if child is not recursive parent.
	 */
    bool SetEntityParent(Entity child, Entity parent);

    static EntityHash HashEntityName(const std::string& entityName);

private:
    Action<Entity> onDestroyEntity;
    Action<Entity, Entity, Entity> onChangeParent;
    std::vector<Entity> parentEntities_;
    std::vector<EntityMask> entityMaskArray_;
    std::vector<EntityHash> entityHashArray_;
};

class DirtyManager
{
public:
    explicit DirtyManager(EntityManager& entityManager);
    DirtyManager( const DirtyManager & ) = default;
    void SetDirty(Entity entity);

    void UpdateDirtyEntities();
	
    template<typename T, EntityMask componentType>
    void RegisterComponentManager(ComponentManager<T, componentType>* componentManager)
    {
        updateDirtyEntity.RegisterCallback(
            [componentManager](Entity entity) { componentManager->UpdateDirtyComponent(entity); });
    }
	
private:
	
    std::reference_wrapper<EntityManager> entityManager_;
    Action<Entity> updateDirtyEntity;
    std::vector<Entity> dirtyEntities_;
};

class OnChangeParentInterface
{
public:
	virtual ~OnChangeParentInterface() = default;
	virtual void OnChangeParent(Entity entity, Entity newParent, Entity oldParent) = 0;
};

class EntityHierarchy : public OnChangeParentInterface
{
public:
    explicit EntityHierarchy(EntityManager& entityManager);
    void OnChangeParent(Entity entity, Entity newParent, Entity oldParent) override;
    const std::vector<Entity>& GetChildren(Entity entity);
    [[nodiscard]] bool HasChildren(Entity entity);
private:
    EntityManager& entityManager_;
    std::unordered_map<Entity, std::vector<Entity>> entityHierarchyMap_;
};

/**
 * ImGui class that allows to show the active entities and select them
 */
class EntityViewer : public DrawImGuiInterface
{
public:
    explicit EntityViewer(EntityManager& entityManager, EntityHierarchy& entityHierarchy);
    [[nodiscard]] Entity GetSelectedEntity() const { return selectedEntity_; }
	void DrawImGui() override;
protected:
    void DrawEntityHierarchy(neko::Entity entity,
                             bool draw,
                             bool destroy);
    EntityHierarchy& entityHierarchy_;
    EntityManager& entityManager_;
    Entity selectedEntity_ = INVALID_ENTITY;
};

}
