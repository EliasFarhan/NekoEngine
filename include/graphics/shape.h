#pragma once

#include <vector>
#include <SFML/Graphics/ConvexShape.hpp>
#include <engine/globals.h>

namespace neko
{
class GraphicsManager;
struct ShapeDef
{
    sf::Color fillColor = sf::Color::White;
    sf::Color outlineColor = sf::Color::White;
    float outlineThickness=0.0f;
};

class ShapeManager
{
public:
    ShapeManager();
    int AddBox(const sf::Vector2f& pos, const sf::Vector2f& halfSize, const ShapeDef& shapeDef);
    int AddPolygon(const sf::Vector2f& pos, const sf::Vector2f* points, size_t pointNmb, const ShapeDef& shapeDef);
    void CopyPosition(const sf::Vector2f* positions, size_t start, size_t length);
    void PushCommands(GraphicsManager* graphicsManager, size_t start = 0, size_t length = InitEntityNmb);
private:
    //for double buffer
    std::vector<sf::ConvexShape> convexShape[2];
};

}