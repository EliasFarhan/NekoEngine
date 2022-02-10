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
#include "engine/engine.h"
#include <algorithm>
#ifdef TRACY_ENABLE
#include <Tracy.hpp>
#endif
namespace neko
{

EntityManager::EntityManager()
{
    std::lock_guard lock(mutex_);
    entityMaskArray_.reserve(INIT_ENTITY_NMB);
}

EntityMask EntityManager::GetMask(Entity entity)
{
    std::shared_lock lock(mutex_);
    return entityMaskArray_[entity];
}

Entity EntityManager::CreateEntity()
{
    std::lock_guard lock(mutex_);
    const auto entityMaskIt = std::ranges::find_if(entityMaskArray_,[](EntityMask entityMask){
       return  entityMask == INVALID_ENTITY_MASK;
    });
    if(entityMaskIt == entityMaskArray_.end())
    {
        entityMaskArray_.push_back(ENTITY_EXIST_FLAG);
        return static_cast<Entity>(entityMaskArray_.size() - 1);
    }
    else
    {
        const auto entity = static_cast<Entity>(entityMaskIt - entityMaskArray_.begin());
        entityMaskArray_[entity] = ENTITY_EXIST_FLAG;
        return entity;
    }
}

void EntityManager::DestroyEntity(Entity entity)
{
#ifdef TRACY_ENABLE
    ZoneScoped
#endif
    std::lock_guard lock(mutex_);
    entityMaskArray_[entity] = INVALID_ENTITY_MASK;
}

bool EntityManager::HasComponent(Entity entity, EntityMask componentType)
{
    std::shared_lock lock(mutex_);
	if (entity >= entityMaskArray_.size())
		return false;
    return (entityMaskArray_[entity] & componentType) == componentType;
}

void EntityManager::AddComponentType(Entity entity, EntityMask componentType)
{
    std::lock_guard lock(mutex_);
    entityMaskArray_[entity] |= componentType;
}

void EntityManager::RemoveComponentType(Entity entity, EntityMask componentType)
{
    std::lock_guard lock(mutex_);
    entityMaskArray_[entity] &= ~componentType;
}

size_t EntityManager::GetEntitiesNmb(EntityMask filterComponents)
{
    std::shared_lock lock(mutex_);
    return std::ranges::count_if(entityMaskArray_,[&filterComponents](EntityMask entityMask){
        return entityMask != INVALID_ENTITY_MASK && (entityMask & filterComponents) == filterComponents;
    });
}

std::vector<Entity, StandardAllocator<Entity>> EntityManager::FilterEntities(EntityMask filterComponents)
{
#ifdef TRACY_ENABLE
    ZoneScoped;
#endif
    std::shared_lock lock(mutex_);
    auto* engine = MainEngine::GetInstance();
    std::vector<Entity, StandardAllocator<Entity>> entities{ StandardAllocator<Entity>{engine->GetFrameAllocator()} };
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
    std::shared_lock lock(mutex_);
    return entityMaskArray_[entity] != INVALID_ENTITY_MASK;
}
}
