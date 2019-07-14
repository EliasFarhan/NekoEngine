#include <multi/graphics/shape.h>
#include <Remotery.h>
#include <multi/engine/engine.h>

namespace multi
{

ShapeManager::ShapeManager()
{
    convexShape_[0].reserve(neko::INIT_ENTITY_NMB);
    convexShape_[1].reserve(neko::INIT_ENTITY_NMB);
}


uint32_t ShapeManager::AddBox(const sf::Vector2f& pos, const sf::Vector2f& halfSize, const neko::ShapeDef& shapeDef)
{
    sf::Vector2f points[4] =
            {
                    -halfSize,
                    sf::Vector2f(-halfSize.x, halfSize.y),
                    halfSize,
                    sf::Vector2f(halfSize.x, -halfSize.y)
            };
    return AddPolygon(pos, points, 4, shapeDef);

}

uint32_t
ShapeManager::AddPolygon(const sf::Vector2f& pos, const sf::Vector2f* points, size_t pointNmb, const neko::ShapeDef& shapeDef)
{
    sf::ConvexShape newPolygon;
    newPolygon.setPointCount(pointNmb);
    newPolygon.setPosition(pos);
    for (auto i = 0u; i < pointNmb; i++)
    {
        newPolygon.setPoint(i, points[i]);
    }
    newPolygon.setFillColor(shapeDef.fillColor);
    newPolygon.setOutlineColor(shapeDef.outlineColor);
    newPolygon.setOutlineThickness(shapeDef.outlineThickness);

    const neko::Index index = neko::Index(convexShape_[0].size());
    convexShape_[0].push_back(newPolygon);
    convexShape_[1].push_back(newPolygon);
    return index;
}



void ShapeManager::PushCommands(neko::GraphicsManager* graphicsManager, size_t start, size_t length)
{
    rmt_ScopedCPUSample(PushShapeCommands, 0);
    const int frameIndex = MainEngine::GetFrameIndex() % 2;
    for (auto i = start; i < start + length; i++)
    {
        graphicsManager->Draw(convexShape_[frameIndex][i]);
    }
}

void ShapeManager::CopyTransformPosition(neko::Position2dManager& positionManager, size_t start, size_t length)
{
    rmt_ScopedCPUSample(CopySpritePositions, 0);
    const int frameIndex = MainEngine::GetFrameIndex() % 2;
    for (auto i = start; i < start + length; i++)
    {
        convexShape_[frameIndex][i].setPosition(positionManager.GetComponent(i));
    }
}

}