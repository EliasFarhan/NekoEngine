#pragma once
#include <vector>

namespace neko
{
using Entity = unsigned;
using EntityMask = unsigned;
enum class ComponentType : unsigned;
const Entity INVALID_ENTITY = 0U;
class EntityManager
{
public:
	EntityManager();
	EntityMask GetMask(Entity entity);
	Entity CreateEntity(Entity wantedEntity = 0u);
	void DestroyEntity(Entity entity);
	bool HasComponent(Entity entity, ComponentType componentType);
	void AddComponentType(Entity entity, ComponentType componentType);
	void RemoveComponentType(Entity entity, ComponentType componentType);
	std::vector<EntityMask> m_MaskArray;
};
}
