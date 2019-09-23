/*
 MIT License

 Copyright (c) 2019 SAE Institute Switzerland AG

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

namespace neko
{

EntityManager::EntityManager()
{
    entityMaskArray_.resize(INIT_ENTITY_NMB);
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
            AddComponentType(Entity(newEntity), EntityMask(NekoComponentType::EMPTY));
            return Entity(newEntity);
        }
        else
        {
            const auto newEntity = entityMaskIt - entityMaskArray_.begin();
            AddComponentType(Entity(newEntity), EntityMask(NekoComponentType::EMPTY));
            return Entity(newEntity);
        }
    }
    else
    {
        ResizeIfNecessary(entityMaskArray_, entity, INVALID_ENTITY_MASK);
        if(!EntityExists(entity))
        {
            AddComponentType(entity, EntityMask(NekoComponentType::EMPTY));
            return entity;
        }
        else
        {
            return CreateEntity(INVALID_ENTITY);
        }
    }
}

void EntityManager::DestroyEntity(Entity entity)
{
    entityMaskArray_[entity] = INVALID_ENTITY_MASK;
}

bool EntityManager::HasComponent(Entity entity, EntityMask componentType) const
{
	if (entity >= entityMaskArray_.size())
		return false;
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

std::vector<Entity> EntityManager::FilterEntities(EntityMask filterComponents)
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
    return HasComponent(entity, EntityMask(NekoComponentType::PREFAB));
}

}
