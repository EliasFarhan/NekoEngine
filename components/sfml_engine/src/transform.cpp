//
// Created by efarhan on 10/11/19.
//
#include <SFML/Graphics/Transform.hpp>
#include "sfml_engine/transform.h"
#include "sfml_engine/physics.h"
#include <glm/glm.hpp>

namespace neko::sfml
{

bool Transform2dManager::SetTransformParent(Entity entity, Entity parentEntity)
{
    //Check if not creating circuit
    if (!CanParentTransform(entity, parentEntity))
        return false;
    transformHierarchy_[entity] = parentEntity;
    return true;
}

Transform2dManager::Transform2dManager(Position2dManager& positionManager, Scale2dManager& scaleManager,
	Rotation2dManager& rotationManager) : positionManager_(positionManager), scaleManager_(scaleManager), rotationManager_(rotationManager)
{
	transformHierarchy_.resize(INIT_ENTITY_NMB, INVALID_ENTITY);
}

bool Transform2dManager::CanParentTransform(Entity entity, Entity parentEntity)
{
    if (entity == neko::INVALID_ENTITY)
    {
        return false;
    }
    if (parentEntity == neko::INVALID_ENTITY)
    {
        return true;
    }
    ResizeIfNecessary(transformHierarchy_, entity, INVALID_ENTITY);
    ResizeIfNecessary(transformHierarchy_, parentEntity, INVALID_ENTITY);
    Entity currentCheckedEntity = parentEntity;
    do
    {
        currentCheckedEntity = transformHierarchy_[currentCheckedEntity];
        if (currentCheckedEntity == entity)
        {
            return false;
        }
    } while (currentCheckedEntity != INVALID_ENTITY);
    return true;
}

Entity
Transform2dManager::FindNextChild(Entity parentEntity, Entity entityChild)
{
    auto begin = transformHierarchy_.cbegin();

    if (entityChild != INVALID_ENTITY)
    {
        begin += entityChild + 1;
    }

    const auto it = std::find_if(begin, transformHierarchy_.cend(), [&parentEntity](const Entity& entity) {
        return parentEntity == entity;
    });
    if (it == transformHierarchy_.cend())
        return INVALID_ENTITY;
    return Entity(it - transformHierarchy_.cbegin());

}

void Transform2dManager::CopyAllFromBody2d(EntityManager& entityManager, box2d::Body2dManager& body2dManager)
{
	const auto entityNmb = entityManager.GetEntitiesSize();
	const auto& bodies = body2dManager.GetComponentsVector();

	for (Index i = 0; i < entityNmb; i++)
	{
		if (
			entityManager.HasComponent(i, EntityMask(NekoComponentType::BODY2D)) &&
			entityManager.HasComponent(i, EntityMask(NekoComponentType::ROTATION2D))
			)
		{
			rotationManager_.SetComponent(i, glm::degrees(bodies[i]->GetAngle()));
		}
		if (
			entityManager.HasComponent(i, EntityMask(NekoComponentType::BODY2D)) &&
			entityManager.HasComponent(i, EntityMask(NekoComponentType::POSITION2D))
			)
		{
			positionManager_.SetComponent(i, Vec2f(bodies[i]->GetPosition()));
		}
	}
}

Entity Transform2dManager::GetParentEntity(Entity entity)
{
    ResizeIfNecessary(transformHierarchy_, entity, INVALID_ENTITY);
    return transformHierarchy_[entity];
}

sf::Transform Transform2dManager::CalculateTransform(Entity entity)
{
	sf::Transform transform = sf::Transform::Identity;
	sf::Transform parentTransform = sf::Transform::Identity;

	auto parentEntity = GetParentEntity(entity);
	if(parentEntity != INVALID_ENTITY)
	{
		parentTransform = CalculateTransform(parentEntity);
	}
	sf::Vector2f point = unit2pixel(positionManager_.GetComponent(entity));

	transform = transform.translate(point);
	transform = transform.scale(scaleManager_.GetComponent(entity));
	transform = transform.rotate(rotationManager_.GetComponent(entity));

	transform = parentTransform.combine(transform);
	return transform;
}

}
