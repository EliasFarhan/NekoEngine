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
#include <engine/transform.h>
#include "engine/globals.h"
#include <physics/physics.h>
#include <glm/glm.hpp>

namespace neko
{

Transform2dManager::Transform2dManager()
{
    transformHierarchy_.resize(INIT_ENTITY_NMB, INVALID_ENTITY);
}

bool Transform2dManager::SetTransformParent(Entity entity, Entity parentEntity)
{
    //Check if not creating circuit
    if(!CanParentTransform(entity, parentEntity))
        return false;
    transformHierarchy_[entity] = parentEntity;
    return true;
}

bool Transform2dManager::CanParentTransform(Entity entity, Entity parentEntity)
{
    if(entity == neko::INVALID_ENTITY)
    {
        return false;
    }
    if(parentEntity == neko::INVALID_ENTITY)
    {
        return true;
    }
    ResizeIfNecessary(transformHierarchy_, entity, INVALID_ENTITY);
    ResizeIfNecessary(transformHierarchy_, parentEntity, INVALID_ENTITY);
    Entity currentCheckedEntity = parentEntity;
    do
    {
        currentCheckedEntity = transformHierarchy_[currentCheckedEntity];
        if(currentCheckedEntity == entity)
        {
            return false;
        }
    } while(currentCheckedEntity != INVALID_ENTITY);
    return true;
}

Entity
Transform2dManager::FindNextChild(Entity parentEntity, Entity entityChild)
{
    auto begin = transformHierarchy_.cbegin();

    if(entityChild != INVALID_ENTITY)
    {
        begin += entityChild+1;
    }

    auto it = std::find_if(begin, transformHierarchy_.cend(), [&parentEntity](const Entity& entity){
        return parentEntity == entity;
    });
    if(it == transformHierarchy_.cend())
        return INVALID_ENTITY;
    return Entity(it-transformHierarchy_.cbegin());

}

Entity Transform2dManager::GetParentEntity(Entity entity)
{
    ResizeIfNecessary(transformHierarchy_, entity, INVALID_ENTITY);
    return transformHierarchy_[entity];
}

sf::Transform Transform2dManager::CalculateTransform(Entity entity)
{
    /*sf::Transform transform = sf::Transform::Identity;
    sf::Transform parentTransform = sf::Transform::Identity;

    auto parentEntity = GetParentEntity(entity);
    if(parentEntity != INVALID_ENTITY)
    {
        parentTransform = CalculateTransform(parentEntity);
    }
    sf::Vector2f point = positionManager_.GetConstComponent(entity);

    transform = transform.translate(point);
    transform = transform.scale(scaleManager_.GetConstComponent(entity));
    transform = transform.rotate(rotationManager_.GetConstComponent(entity));

    transform = parentTransform.combine(transform);
    return transform;*/
}

void Rotation2dManager::CopyAnglesFromBody2d(EntityManager& entityManager, Body2dManager& body2dManager)
{
	const auto entityNmb = entityManager.GetEntitiesSize();
	const auto& bodies = body2dManager.GetConstComponentsVector();

	for (Index i = 0; i < entityNmb; i++)
	{
		if (
			entityManager.HasComponent(i, EntityMask(NekoComponentType::BODY2D)) &&
			entityManager.HasComponent(i, EntityMask(NekoComponentType::ROTATION2D))
			)
		{
			components_[i] = glm::degrees(bodies[i]->GetAngle());
		}
	}
}

void Position2dManager::CopyAllPositionsFromBody2d(EntityManager& entityManager, Body2dManager& body2dManager)
{
	const auto entityNmb = entityManager.GetEntitiesSize();
	const auto& bodies = body2dManager.GetConstComponentsVector();
	for(Index i = 0; i < entityNmb;i++)
	{
		if(
			entityManager.HasComponent(i, EntityMask(NekoComponentType::BODY2D)) &&
			entityManager.HasComponent(i, EntityMask(NekoComponentType::POSITION2D)) 
			)
		{
			//components_[i] = meter2pixel(bodies[i]->GetPosition());
		}
	}
}

Position2dManager::Position2dManager() : ComponentManager::ComponentManager()
{
}

Index Scale2dManager::AddComponent(EntityManager& entityManager, Entity entity)
{
    ResizeIfNecessary(components_, entity, sf::Vector2f(1.0f,1.0f));
    return ComponentManager::AddComponent(entityManager, entity);
}

}
