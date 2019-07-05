/*
 MIT License

 Copyright (c) 2017 SAE Institute Switzerland AG

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

namespace neko
{

EntityManager::EntityManager()
{
    entityMaskArray_.reserve(INIT_ENTITY_NMB);
}

EntityMask EntityManager::GetMask(Entity entity)
{
    return entityMaskArray_[entity];
}

Entity EntityManager::CreateEntity()
{
    const auto entityMaskIt = std::find_if(entityMaskArray_.begin(), entityMaskArray_.end(),[](EntityMask entityMask){
       return  entityMask == INVALID_ENTITY_MASK;
    });
    if(entityMaskIt == entityMaskArray_.end())
    {
        entityMaskArray_.push_back(INVALID_ENTITY_MASK);
        return Entity(entityMaskArray_.size()-1);
    }
    else
    {
        return Entity(entityMaskIt-entityMaskArray_.begin());
    }
}

void EntityManager::DestroyEntity(Entity entity)
{
    entityMaskArray_[entity] = INVALID_ENTITY_MASK;
}

bool EntityManager::HasComponent(Entity entity, EntityMask componentType)
{
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

std::vector<Entity> EntityManager::FilterEntities(EntityMask filterComponents)
{
	std::vector<Entity> entities;
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
}
