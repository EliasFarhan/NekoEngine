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
#include "SFML/System/Vector2.hpp"
#include "globals.h"
#include <engine/entity.h>
#include "engine/component.h"
#include "mathematics/vector.h"
#include <SFML/Graphics/Transform.hpp>

namespace neko
{
class Body2dManager;
class Physics2dManager;


class Position2dManager : public ComponentManager<Vec2f, ComponentType(NekoComponentType::POSITION2D)>
{
public:
    Position2dManager();
    void CopyAllPositionsFromBody2d(EntityManager& entityManager, Body2dManager& body2dManager);

};

class Scale2dManager : public ComponentManager<sf::Vector2f, ComponentType(NekoComponentType::SCALE2D)>
{
public:
    using ComponentManager::ComponentManager;
    Index AddComponent(EntityManager& entityManager, Entity entity) override;

};

class Rotation2dManager : public ComponentManager<float, ComponentType(NekoComponentType::ROTATION2D)>
{
public:
    using ComponentManager::ComponentManager;
    void CopyAnglesFromBody2d(EntityManager& entityManager, Body2dManager& body2dManager);
};

/**
 * \brief manage the graphic transform of any kind of objects in a SOA fashions
 */
class Transform2dManager
{
public:
    Transform2dManager();

    Position2dManager& GetPositionManager(){ return positionManager_;}
    Scale2dManager& GetScaleManager() { return scaleManager_; }
    Rotation2dManager& GetRotationManager() { return rotationManager_;}
    bool CanParentTransform(Entity entity, Entity parentEntity);
    bool SetTransformParent(Entity entity, Entity parentEntity);
    Entity GetParentEntity(Entity entity);
    Entity FindNextChild(Entity parentEntity, Entity entityChild=neko::INVALID_ENTITY);

    sf::Transform CalculateTransform(Entity entity);
private:
    Position2dManager positionManager_;
    Scale2dManager scaleManager_{sf::Vector2f(1.0f,1.0f)};
    Rotation2dManager rotationManager_;

	std::vector<Entity> transformHierarchy_;
	std::vector<char> dirtyFlags_;
};

}
