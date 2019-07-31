#pragma once

#include <graphics/shape.h>

namespace multi
{

/**
 * \brief managing graphic shape from the SFML API
 */
class ShapeManager : neko::BadShapeManager
{
public:
    ShapeManager();

    neko::Index AddBox(const sf::Vector2f& pos, const sf::Vector2f& halfSize, const neko::ShapeDef& shapeDef);

    neko::Index AddPolygon(const sf::Vector2f& pos, const sf::Vector2f* points, size_t pointNmb, const neko::ShapeDef& shapeDef);


    void CopyTransformPosition(neko::Position2dManager& positionManager, size_t start = 0,
                               size_t length = neko::INIT_ENTITY_NMB);
    void PushCommands(neko::GraphicsManager* graphicsManager, size_t start = 0, size_t length = neko::INIT_ENTITY_NMB);

private:
    /**
     * \brief storing the shape and putting two vectors for double buffering with the render thread
     */
    std::vector<sf::ConvexShape> convexShape_[2];
};

}