#pragma once

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

#include <vector>
#include <engine/globals.h>

namespace neko
{
/**
 * \brief Entity start at 0 and goes to N
 */
using Entity = Index;
/**
 * \brief EntityMask is a bitmask representation of the activated components
 */
using EntityMask = std::uint32_t;
enum class ComponentType : unsigned;
const Entity INVALID_ENTITY = std::numeric_limits<Index>::max();
const EntityMask INVALID_ENTITY_MASK = 0u;

/**
 * \brief Used in an Entity-Component-System to store all entities and what components they have
 */
class EntityManager
{
public:
	EntityManager();

	EntityMask GetMask(Entity entity);

	Entity CreateEntity();

	void DestroyEntity(Entity entity);

	bool HasComponent(Entity entity, EntityMask componentType);

	bool EntityExists(Entity entity);

	size_t GetEntitiesNmb(EntityMask filterComponents = INVALID_ENTITY_MASK);

	std::vector<Entity> FilterEntities(EntityMask filterComponents = INVALID_ENTITY_MASK);

	void AddComponentType(Entity entity, EntityMask componentType);

	void RemoveComponentType(Entity entity, EntityMask componentType);
private:
	mutable std::shared_mutex mutex_;
	std::vector<EntityMask> entityMaskArray_;
};

struct Component
{
	Entity entity = INVALID_ENTITY;
};
}
