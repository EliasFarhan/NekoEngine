#pragma once

/*
 MIT License

 Copyright (c) 2017 SAE Institute Switzerland AG

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
class MultiThreadGraphicsManager;

struct ShapeDef
{
    sf::Color fillColor = sf::Color::White;
    sf::Color outlineColor = sf::Color::White;
    float outlineThickness = 0.0f;
};

/**
 * \brief managing graphic shape from the SFML API
 */
class ShapeManager
{
public:
    ShapeManager();

    Index AddBox(const sf::Vector2f& pos, const sf::Vector2f& halfSize, const ShapeDef& shapeDef);

    Index AddPolygon(const sf::Vector2f& pos, const sf::Vector2f* points, size_t pointNmb, const ShapeDef& shapeDef);


    void CopyTransformPosition(OldTransform2dManager& transformManager, size_t start = 0, size_t length = INIT_ENTITY_NMB);
    void PushCommands(MultiThreadGraphicsManager* graphicsManager, size_t start = 0, size_t length = INIT_ENTITY_NMB);

private:
    /**
     * \brief storing the shape and putting two vectors for double buffering with the render thread
     */
    std::vector<sf::ConvexShape> convexShape_[2];
};

}