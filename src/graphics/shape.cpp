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
ShapeManager::ShapeManager()
{
    convexShape_[0].reserve(InitEntityNmb);
    convexShape_[1].reserve(InitEntityNmb);
}


uint32_t ShapeManager::AddBox(const sf::Vector2f& pos, const sf::Vector2f& halfSize, const ShapeDef& shapeDef)
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
ShapeManager::AddPolygon(const sf::Vector2f& pos, const sf::Vector2f* points, size_t pointNmb, const ShapeDef& shapeDef)
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

    const Index index = convexShape_[0].size();
    convexShape_[0].push_back(newPolygon);
    convexShape_[1].push_back(newPolygon);
    return index;
}



void ShapeManager::PushCommands(GraphicsManager* graphicsManager, size_t start, size_t length)
{
    rmt_ScopedCPUSample(PushShapeCommands, 0);
    const int frameIndex = MainEngine::GetInstance()->frameIndex % 2;
    for (auto i = start; i < start + length; i++)
    {
        graphicsManager->Draw(convexShape_[frameIndex][i]);
    }
}

void ShapeManager::CopyTransformPosition(Transform2dManager& transformManager, size_t start, size_t length)
{
    rmt_ScopedCPUSample(CopySpritePositions, 0);
    const int frameIndex = MainEngine::GetInstance()->frameIndex % 2;
    for (auto i = start; i < start + length; i++)
    {
        convexShape_[frameIndex][i].setPosition(transformManager.positions_[i]);
    }
}

}