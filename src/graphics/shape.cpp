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
#include <graphics/shape.h>
#include <engine/globals.h>
#include <engine/engine.h>
#include <Remotery.h>

namespace neko
{


void ConvexShapeManager::AddBox(Entity entity, const Vec2f& pos, const Vec2f& halfSize, const ShapeDef& shapeDef)
{
    Vec2f points[4] =
    {
        Vec2f()-halfSize,
        sf::Vector2f(-halfSize.x, halfSize.y),
        halfSize,
        sf::Vector2f(halfSize.x, -halfSize.y)
    };
    AddPolygon(entity, pos, points, 4, shapeDef);
}

void ConvexShapeManager::AddPolygon(Entity entity, const Vec2f& pos, const Vec2f* points, size_t pointNmb,
                                    const ShapeDef& shapeDef)
{
    sf::ConvexShape newPolygon;
    newPolygon.setPointCount(pointNmb);
    newPolygon.setPosition(unit2pixel(pos));
    for (auto i = 0u; i < pointNmb; i++)
    {
        newPolygon.setPoint(i, unit2pixel(points[i]));
    }
    newPolygon.setFillColor(shapeDef.fillColor);
    newPolygon.setOutlineColor(shapeDef.outlineColor);
    newPolygon.setOutlineThickness(shapeDef.outlineThickness);
    ResizeIfNecessary(components_, entity, sf::ConvexShape());
    components_[entity] = newPolygon;
}

void ConvexShapeManager::CopyTransformPosition(Position2dManager& positionManager, size_t start, size_t length)
{
	const auto positions = positionManager.GetConstComponentsVector();
    for(auto i = start; i < start+length; i++)
    {
        components_[i].setPosition(neko::unit2pixel(positions[i]));
    }
}

void ConvexShapeManager::CopyTransformRotation(Rotation2dManager& rotationManager, size_t start, size_t length)
{
	const auto rotations = rotationManager.GetConstComponentsVector();
	for (auto i = start; i < start + length; i++)
	{
		components_[i].setRotation(-rotations[i]); //counter clockwise rotation
	}
}

void ConvexShapeManager::PushCommands(GraphicsManager* graphicsManager, size_t start, size_t length)
{
    for(auto i = start; i < start+length;i++)
    {
        graphicsManager->Draw(components_[i]);
    }
}
}