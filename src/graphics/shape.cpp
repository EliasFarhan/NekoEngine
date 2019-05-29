#include <graphics/shape.h>
#include <engine/globals.h>
#include <engine/engine.h>

#include <Remotery.h>

namespace neko
{
ShapeManager::ShapeManager()
{
    convexShape[0].reserve(InitEntityNmb);
    convexShape[1].reserve(InitEntityNmb);
}



int ShapeManager::AddBox(const sf::Vector2f& pos, const sf::Vector2f& halfsize, const ShapeDef& shapeDef)
{
    sf::Vector2f points[4] =
    {
        -halfsize,
        sf::Vector2f(-halfsize.x, halfsize.y),
        halfsize,
        sf::Vector2f(halfsize.x, -halfsize.y)
    };
    return AddPolygon(pos, points, 4, shapeDef);

}

int ShapeManager::AddPolygon(const sf::Vector2f &pos, const sf::Vector2f *points, size_t pointNmb, const ShapeDef& shapeDef)
{
    sf::ConvexShape newPolygon;
    newPolygon.setPointCount(pointNmb);
    newPolygon.setPosition(pos);
    for(auto i = 0u; i < pointNmb; i++)
    {
        newPolygon.setPoint(i,points[i]);
    }
    newPolygon.setFillColor(shapeDef.fillColor);
    newPolygon.setOutlineColor(shapeDef.outlineColor);
    newPolygon.setOutlineThickness(shapeDef.outlineThickness);

    int index = convexShape[0].size();
    convexShape[0].push_back(newPolygon);
    convexShape[1].push_back(newPolygon);
    return index;
}

void ShapeManager::CopyPosition(const sf::Vector2f* positions, size_t start, size_t length)
{
    rmt_ScopedCPUSample(CopyShapePositions, 0);
    for(auto i = start; i < start+length; i++)
    {
        convexShape[0][i].setPosition(positions[i-start]);
        convexShape[1][i].setPosition(positions[i-start]);
    }
}

void ShapeManager::PushCommands(GraphicsManager *graphicsManager, size_t start, size_t length)
{
    rmt_ScopedCPUSample(PushShapeCommands, 0);
    const int frameIndex = MainEngine::GetInstance()->frameIndex%2;
    for (auto i = start; i < start + length; i++)
    {
        graphicsManager->Draw(convexShape[frameIndex][i]);
    }
}

}