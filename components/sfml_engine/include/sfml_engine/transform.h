#pragma once

#include "engine/transform.h"
namespace neko
{
namespace box2d
{
class Body2dManager;
}
namespace sfml
{
class Transform2dManager
{
public:
	Transform2dManager() = delete;
    Transform2dManager(Position2dManager& positionManager, Scale2dManager& scaleManager, Rotation2dManager& rotationManager);

    bool CanParentTransform(Entity entity, Entity parentEntity);
    bool SetTransformParent(Entity entity, Entity parentEntity);
    Entity GetParentEntity(Entity entity);
    Entity FindNextChild(Entity parentEntity, Entity entityChild=neko::INVALID_ENTITY);
	void CopyAllFromBody2d(EntityManager& entityManager, box2d::Body2dManager& body2dManager);
    sf::Transform CalculateTransform(Entity entity);

	



private:

	Position2dManager& positionManager_;
	Scale2dManager& scaleManager_;//{ Vec2f(1.0f,1.0f) }
	Rotation2dManager& rotationManager_;
    std::vector<Entity> transformHierarchy_;
    std::vector<char> dirtyFlags_;
};

}
}