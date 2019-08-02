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
}

//TODO Benchmark copy entites before apply or filter on loop
void Angle2dManager::CopyAnglesFromBody2d(EntityManager& entityManager, Body2dManager& body2dManager)
{
	const auto entityNmb = entityManager.GetEntitiesSize();
	const auto& bodies = body2dManager.GetConstComponentsVector();

	for (Index i = 0; i < entityNmb; i++)
	{
		if (
			entityManager.HasComponent(i, EntityMask(NekoComponentType::BODY2D)) &&
			entityManager.HasComponent(i, EntityMask(NekoComponentType::ANGLE2D))
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
			components_[i] = meter2pixel(bodies[i]->GetPosition());
		}
	}
}

Index Scale2dManager::AddComponent(EntityManager& entityManager, Entity entity)
{
    ResizeIfNecessary(components_, entity, sf::Vector2f(1.0f,1.0f));
    return ComponentManager::AddComponent(entityManager, entity);
}

}
