#pragma once

#include <engine/component.h>
#include <Box2D/Box2D.h>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>

namespace neko
{
class GraphicsManager;
}
namespace editor
{
class NekoEditor;
template<class TDef, class TShape>
struct Collider
{
    b2FixtureDef fixtureDef;
    TDef shapeDef{};
    TShape shape{};

};

struct BoxCollider : Collider<b2PolygonShape, sf::RectangleShape>
{
    sf::Vector2f size = sf::Vector2f(1.0f,1.0f);
    sf::Vector2f offset = sf::Vector2f();
};

struct CircleCollider : Collider<b2CircleShape, sf::CircleShape>
{
    sf::Vector2f offset = sf::Vector2f();
};

struct PolygonCollider : Collider<b2PolygonShape, sf::ConvexShape>
{

};

class BoxColliderDefManager : public neko::ComponentManager
        <BoxCollider, neko::ComponentType(neko::NekoComponentType::BOXCOLLIDER2D)>
{

};
class CircleColliderDefManager : public neko::ComponentManager
        <CircleCollider, neko::ComponentType(neko::NekoComponentType::CIRCLECOLLIDER2D)>
{
public:
    neko::Index AddComponent(neko::EntityManager& entityManager, neko::Entity entity) override;

};

class PolygonColldierDefManager : public neko::ComponentManager
        <PolygonCollider, neko::ComponentType(neko::NekoComponentType::POLYGONCOLLIDER2D)>
{

};

class ColliderDefManager
{
public:
    explicit ColliderDefManager(NekoEditor& nekoEditor);
    void PushAllCommands(neko::GraphicsManager& graphicsManager);

    BoxColliderDefManager& GetBoxColliderDefManager();
    CircleColliderDefManager& GetCircleColliderDefManager();
    PolygonColldierDefManager& GetPolygonColliderDefManager();
private:
    NekoEditor& nekoEditor_;

    BoxColliderDefManager boxColliderDefManager_;
    CircleColliderDefManager circleColliderDefManager_;
    PolygonColldierDefManager polygonColldierDefManager_;



};
}