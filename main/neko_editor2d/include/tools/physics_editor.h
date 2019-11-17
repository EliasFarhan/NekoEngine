#pragma once

#include <engine/component.h>
#include <Box2D/Box2D.h>
#include <SFML/Graphics/Shape.hpp>
#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/ConvexShape.hpp>
#include "sfml_engine/graphics.h"

namespace neko {
class GraphicsManager;
}

namespace neko::sfml
{
class Transform2dManager;

}
namespace neko::editor
{
struct NekoEditorExport;
class NekoEditor;
template<class TDef>
struct Collider
{
    b2FixtureDef fixtureDef;
    TDef shapeDef{};

};

struct BoxCollider : Collider<b2PolygonShape>
{
    Vec2f size = Vec2f(1.0f,1.0f);
    Vec2f offset = Vec2f();
};

struct CircleCollider : Collider<b2CircleShape>
{
    Vec2f offset = Vec2f();
};

struct PolygonCollider : Collider<b2PolygonShape>
{

};

class BoxColliderDefManager : public neko::ComponentManager
        <BoxCollider, neko::ComponentType(neko::NekoComponentType::BOX_COLLIDER2D)>
{
public:
	using ComponentManager::ComponentManager;
};
class CircleColliderDefManager : public neko::ComponentManager
        <CircleCollider, neko::ComponentType(neko::NekoComponentType::CIRCLE_COLLIDER2D)>
{
public:

	using ComponentManager::ComponentManager;
    neko::Index AddComponent(neko::EntityManager& entityManager, neko::Entity entity) override;

};

class PolygonColldierDefManager : public neko::ComponentManager
        <PolygonCollider, neko::ComponentType(neko::NekoComponentType::POLYGON_COLLIDER2D)>
{
public:

	using ComponentManager::ComponentManager;
	Index AddComponent(EntityManager& entityManager, Entity entity) override;
};

template<class TShape>
struct DebugShapeCommand
{
	TShape shape;
	sf::RenderStates states = sf::RenderStates::Default;
};

class ColliderDefManager
{
public:
    explicit ColliderDefManager(NekoEditorExport& editorExport);
    void PushAllCommands(GraphicsManager& graphicsManager);
private:
	std::vector<sfml::SfmlRenderCommand> commands_;
	std::vector<DebugShapeCommand<sf::RectangleShape>> rectShapes_;
	std::vector<DebugShapeCommand<sf::CircleShape>> circleShapes_;
	std::vector<DebugShapeCommand<sf::ConvexShape>> convexShapes_;
	
    BoxColliderDefManager& boxColliderDefManager_;
    CircleColliderDefManager& circleColliderDefManager_;
    PolygonColldierDefManager& polygonColldierDefManager_;
    EntityManager& entityManager_;
    sfml::Transform2dManager& transformManager_;
};
}