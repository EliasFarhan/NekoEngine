#include <engine/entity.h>
#include "engine/globals.h"

namespace neko
{

EntityManager::EntityManager()
{
	m_MaskArray.reserve(InitEntityNmb);
}

EntityMask EntityManager::GetMask(Entity entity)
{
	return m_MaskArray[entity - 1];
}

Entity EntityManager::CreateEntity(Entity wantedEntity)
{
	for (Entity entity = 1U; entity <= m_MaskArray.size(); entity++)
	{
		if (m_MaskArray[entity - 1] == INVALID_ENTITY)
		{
			return entity;
		}
	}
	return INVALID_ENTITY;
}

void EntityManager::DestroyEntity(Entity entity)
{
	m_MaskArray[entity - 1] = INVALID_ENTITY;
}

bool EntityManager::HasComponent(Entity entity, ComponentType componentType)
{
	return (m_MaskArray[entity - 1] & static_cast<EntityMask>(componentType)) == static_cast<EntityMask>(componentType);
}

void EntityManager::AddComponentType(Entity entity, ComponentType componentType)
{
	m_MaskArray[entity - 1] = m_MaskArray[entity - 1] | static_cast<EntityMask>(componentType);
}

void EntityManager::RemoveComponentType(Entity entity, ComponentType componentType)
{
	m_MaskArray[entity - 1] &= ~static_cast<EntityMask>(componentType);
}
}
