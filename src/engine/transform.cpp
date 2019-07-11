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

OldTransform2dManager::OldTransform2dManager()
{
	positions_.resize(INIT_ENTITY_NMB);
	scales_.resize(INIT_ENTITY_NMB);
	angles_.resize(INIT_ENTITY_NMB);
}

void OldTransform2dManager::CopyPositionsFromPhysics2d(Physics2dManager& physics2dManager, size_t start, size_t length)
{
	for (auto i = start; i < start + length; i++)
	{
		positions_[i] = meter2pixel(physics2dManager.bodies_[i]->GetPosition());
	}
}

void OldTransform2dManager::CopyAnglesFromPhysics2d(Physics2dManager& physics2dManager, size_t start, size_t length)
{
	for (auto i = start; i < start + length; i++)
	{
		angles_[i] = glm::degrees(physics2dManager.bodies_[i]->GetAngle());
	}
}

Index OldTransform2dManager::AddPosition(sf::Vector2f position, Entity entity)
{
	if (entity == INVALID_ENTITY)
	{
		positions_.push_back(position);
		return Index(positions_.size());
	}
	
	size_t futureSize = positions_.size();
	if (futureSize <= entity)
	{
		while (futureSize <= entity)
		{
			futureSize *= 2;
		}
		positions_.resize(futureSize);
	}
	positions_[entity] = position;
	return entity;
}

Index OldTransform2dManager::AddScale(sf::Vector2f scale, Entity entity)
{
	if (entity == INVALID_ENTITY)
	{
		scales_.push_back(scale);
		return Index(scales_.size());
	}
	if(scales_.size() <= entity)
	{
		scales_.resize(size_t(entity) + 1);
	}
	scales_[entity] = scale;
	return entity;
}

Index OldTransform2dManager::AddAngle(float angle, Entity entity)
{
	if (entity == INVALID_ENTITY)
	{
		angles_.push_back(angle);
		return Index(angles_.size());
	}
	if(angles_.size() <= entity)
	{
		angles_.resize(size_t(entity) + 1);
	}
	angles_[entity] = angle;
	return entity;
}

sf::Vector2f OldTransform2dManager::GetPosition(Index i) const
{
	return positions_[i];
}

void OldTransform2dManager::SetPosition(const sf::Vector2f& position, Index i)
{
	positions_[i] = position;
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

void Position2dManager::CopyPositionsFromBody2d(EntityManager& entityManager, Body2dManager& body2dManager)
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
}
