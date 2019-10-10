#include <tools/physics_editor.h>
#include <graphics/graphics.h>
#include <tools/neko_editor.h>
#include <engine/transform.h>

namespace editor
{
const static int physicsLayer = 100;
const static float outlineThickness = 2.0f;


ColliderDefManager::ColliderDefManager(NekoEditor& nekoEditor) : nekoEditor_(nekoEditor)
{

}

void ColliderDefManager::PushAllCommands(neko::GraphicsManager& graphicsManager)
{
    const auto boxEntityMask = neko::EntityMask(neko::NekoComponentType::BOX_COLLIDER2D);
    const auto circleEntityMask = neko::EntityMask(neko::NekoComponentType::CIRCLE_COLLIDER2D);
    const auto polygonEntityMask = neko::EntityMask(neko::NekoComponentType::POLYGON_COLLIDER2D);
    const auto& entityManager = nekoEditor_.GetEntityManager();
    auto& transformManager = nekoEditor_.GetTransformManager();
    for(neko::Entity entity = 0; entity < entityManager.GetEntitiesSize();entity++)
    {
        if(entityManager.HasComponent(entity, boxEntityMask))
        {
            //Draw box
            auto& boxColliderDef = boxColliderDefManager_.GetComponent(entity);
            boxColliderDef.shape.setSize(neko::unit2pixel(boxColliderDef.size));
            boxColliderDef.shape.setOrigin(boxColliderDef.shape.getSize()/2.0f);
            boxColliderDef.shape.setPosition(neko::unit2pixel(boxColliderDef.offset));
            boxColliderDef.shape.setOutlineColor(sf::Color::Green);
            boxColliderDef.shape.setOutlineThickness(outlineThickness);
            boxColliderDef.shape.setFillColor(sf::Color::Transparent);

            sf::RenderStates states;
            auto transform = transformManager.CalculateTransform(entity);
            states.transform = transform;
            graphicsManager.Draw(boxColliderDef.shape, physicsLayer, states);
        }
        if(entityManager.HasComponent(entity, circleEntityMask))
        {
            //Draw circle
            auto& circleColliderDef = circleColliderDefManager_.GetComponent(entity);
            const float radius = neko::meter2pixel(circleColliderDef.shapeDef.m_radius);
            circleColliderDef.shape.setRadius(radius);
            circleColliderDef.shape.setOrigin(sf::Vector2f(radius, radius));
            circleColliderDef.shape.setPosition(neko::unit2pixel(circleColliderDef.offset));
            circleColliderDef.shape.setOutlineColor(sf::Color::Green);
            circleColliderDef.shape.setOutlineThickness(outlineThickness);
            circleColliderDef.shape.setFillColor(sf::Color::Transparent);

            sf::RenderStates states;
            auto transform = transformManager.CalculateTransform(entity);
            states.transform = transform;
            graphicsManager.Draw(circleColliderDef.shape, physicsLayer, states);
        }
        if(entityManager.HasComponent(entity, polygonEntityMask))
        {
            //Draw polygon
        }
    }
}

BoxColliderDefManager& ColliderDefManager::GetBoxColliderDefManager()
{
    return boxColliderDefManager_;
}

CircleColliderDefManager& ColliderDefManager::GetCircleColliderDefManager()
{
    return circleColliderDefManager_;
}

PolygonColldierDefManager& ColliderDefManager::GetPolygonColliderDefManager()
{
    return polygonColldierDefManager_;
}

neko::Index CircleColliderDefManager::AddComponent(neko::EntityManager& entityManager, neko::Entity entity)
{
    auto index = ComponentManager::AddComponent(entityManager, entity);
    auto& circleColliderDef = GetComponent(entity);
    circleColliderDef.shapeDef.m_radius = 1.0f;
    return index;
}
}