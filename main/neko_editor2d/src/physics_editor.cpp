#include <tools/physics_editor.h>
#include <graphics/graphics.h>
#include <tools/neko_editor.h>
#include <engine/transform.h>

namespace neko::editor
{
const static int physicsLayer = 100;
const static float outlineThickness = 2.0f;



void ColliderDefManager::PushAllCommands(GraphicsManager& graphicsManager)
{
	const auto boxEntityMask = neko::EntityMask(neko::NekoComponentType::BOX_COLLIDER2D);
	const auto circleEntityMask = neko::EntityMask(neko::NekoComponentType::CIRCLE_COLLIDER2D);
	const auto polygonEntityMask = neko::EntityMask(neko::NekoComponentType::POLYGON_COLLIDER2D);

	rectShapes_.clear();
	circleShapes_.clear();

	for (neko::Entity entity = 0; entity < entityManager_.GetEntitiesSize(); entity++)
	{
		if (entityManager_.HasComponent(entity, boxEntityMask))
		{
			//Render box
			const auto& boxColliderDef = boxColliderDefManager_.GetComponent(entity);
			sf::RectangleShape shape;
			shape.setSize(unit2pixel(boxColliderDef.size));
			shape.setOrigin(shape.getSize() / 2.0f);
			shape.setPosition(unit2pixel(boxColliderDef.offset));
			shape.setOutlineColor(sf::Color::Green);
			shape.setOutlineThickness(outlineThickness);
			shape.setFillColor(sf::Color::Transparent);

			
			
			sf::RenderStates states;
			auto transform = transformManager_.CalculateTransform(entity);
			states.transform = transform;
			rectShapes_.push_back({ shape, states });
		}
		if (entityManager_.HasComponent(entity, circleEntityMask))
		{
			//Render circle
			const auto& circleColliderDef = circleColliderDefManager_.GetComponent(entity);
			sf::CircleShape shape;
			const float radius = box2d::meter2pixel(circleColliderDef.shapeDef.m_radius);
			shape.setRadius(radius);
			shape.setOrigin(sf::Vector2f(radius, radius));
			shape.setPosition(neko::unit2pixel(circleColliderDef.offset));
			shape.setOutlineColor(sf::Color::Green);
			shape.setOutlineThickness(outlineThickness);
			shape.setFillColor(sf::Color::Transparent);
			sf::RenderStates states;
			const auto transform = transformManager_.CalculateTransform(entity);
			states.transform = transform;
			circleShapes_.push_back({ shape, states });
		}
		if (entityManager_.HasComponent(entity, polygonEntityMask))
		{
			//TODO Render polygon
		}
	}

	commands_.clear();
	commands_.reserve(rectShapes_.size() + circleShapes_.size() + convexShapes_.size());
	for(auto& rectShape : rectShapes_)
	{
		sfml::SfmlRenderCommand command;
		command.SetDrawable(&rectShape.shape);
		command.SetStates(rectShape.states);
		command.SetLayer(physicsLayer);
		commands_.push_back(command);
	}
	for(auto& circleShape : circleShapes_)
	{
		sfml::SfmlRenderCommand command;
		command.SetDrawable(&circleShape.shape);
		command.SetStates(circleShape.states);
		command.SetLayer(physicsLayer);
		commands_.push_back(command);
	}
	for(auto& convexShape : convexShapes_)
	{
		sfml::SfmlRenderCommand command;
		command.SetDrawable(&convexShape.shape);
		command.SetStates(convexShape.states);
		command.SetLayer(physicsLayer);
		commands_.push_back(command);
	}

	for(auto& command : commands_)
    {
	    graphicsManager.Render(&command);
    }
}

neko::Index CircleColliderDefManager::AddComponent(neko::EntityManager& entityManager, neko::Entity entity)
{
	auto index = ComponentManager::AddComponent(entityManager, entity);
	auto tmpCircleColliderDef = GetComponent(entity);
    tmpCircleColliderDef.shapeDef.m_radius = 1.0f;
    SetComponent(entity, tmpCircleColliderDef);
	return index;
}

Index PolygonColldierDefManager::AddComponent(EntityManager& entityManager, Entity entity)
{
    //TODO Implement for polygon
    (void)entityManager;
    (void)entity;
    return INVALID_INDEX;
}

ColliderDefManager::ColliderDefManager(NekoEditorExport& editorExport) :
	boxColliderDefManager_(editorExport.boxColliderDefManager),
	circleColliderDefManager_(editorExport.circleColliderDefManager),
	polygonColldierDefManager_(editorExport.polygonColldierDefManager),
	entityManager_(editorExport.entityManager),
	transformManager_(editorExport.transform2dManager)
{
}
}
