#pragma once

/*
 MIT License

 Copyright (c) 2019 SAE Institute Switzerland AG

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
#include <SFML/Graphics/ConvexShape.hpp>
#include <engine/globals.h>
#include <engine/transform.h>

namespace neko
{
class GraphicsManager;

struct ShapeDef
{
    sf::Color fillColor = sf::Color::White;
    sf::Color outlineColor = sf::Color::White;
    float outlineThickness = 0.0f;
};

/**
 * \brief managing graphic shape from the SFML API
 */
class BadShapeManager
{
public:
    BadShapeManager() = default;

    virtual Index AddBox(const sf::Vector2f& pos, const sf::Vector2f& halfSize, const ShapeDef& shapeDef) = 0;

    virtual Index AddPolygon(const sf::Vector2f& pos, const sf::Vector2f* points, size_t pointNmb, const ShapeDef& shapeDef) = 0;


    virtual void CopyTransformPosition(Position2dManager& positionManager, size_t start = 0,
                                       size_t length = neko::INIT_ENTITY_NMB) = 0;
    virtual void PushCommands(GraphicsManager* graphicsManager, size_t start = 0, size_t length = INIT_ENTITY_NMB) = 0;

};

class ConvexShapeManager : public ComponentManager<sf::ConvexShape, ComponentType(NekoComponentType::CONVEX_SHAPE2D)>
{
public:
    using ComponentManager::ComponentManager;

    void AddBox(Entity entity, const Vec2f& pos, const Vec2f& halfSize, const ShapeDef& shapeDef);

    void AddPolygon(Entity entity, const Vec2f& pos, const Vec2f* points, size_t pointNmb, const ShapeDef& shapeDef);

    void CopyTransformPosition(Position2dManager& positionManager, size_t start, size_t length);
    void CopyTransformRotation(Rotation2dManager& rotationManager, size_t start, size_t length);

    void PushCommands(GraphicsManager* graphicsManager, size_t start, size_t length);


};



}