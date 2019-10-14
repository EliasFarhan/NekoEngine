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


class SfmlPosition2dManager : public Position2dManager
{
public:
    void CopyAllPositionsFromBody2d(EntityManager& entityManager, box2d::Body2dManager& body2dManager);
};

class SfmlRotation2dManager : public Rotation2dManager
{
public:
    void CopyAnglesFromBody2d(EntityManager& entityManager, box2d::Body2dManager& body2dManager);
};
/**
 * \brief manage the graphic transform of any kind of objects in a SOA fashions
 */
struct SfmlTransform2dManager
{
public:
    SfmlTransform2dManager();

    bool CanParentTransform(Entity entity, Entity parentEntity);
    bool SetTransformParent(Entity entity, Entity parentEntity);
    Entity GetParentEntity(Entity entity);
    Entity FindNextChild(Entity parentEntity, Entity entityChild=neko::INVALID_ENTITY);

    sf::Transform CalculateTransform(Entity entity);


    SfmlPosition2dManager positionManager_;
    Scale2dManager scaleManager_{Vec2f(1.0f,1.0f)};
    SfmlRotation2dManager rotationManager_;

private:


    std::vector<Entity> transformHierarchy_;
    std::vector<char> dirtyFlags_;
};

}
}