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
#include <city_tilemap.h>
#include <graphics/texture.h>
#include <engine/engine.h>
#include <Remotery.h>
#include <city_map.h>

namespace neko
{
void CityBuilderTilemap::Init(TextureManager& textureManager)
{
    for(int i = 0; i < int(CityTilesheetType::CAR); i++)
    {
		Index textureIndex = textureManager.LoadTexture(cityTextureName);
		tilesheets[i].texture = textureManager.GetTexture(textureIndex);
        tilesheets[i].texture->setRepeated(true);
    }
	const Index textureIndex = textureManager.LoadTexture(carTextureName);
	tilesheets[unsigned(CityTilesheetType::CAR)].texture = textureManager.GetTexture(textureIndex);
    tilesheets[unsigned(CityTilesheetType::CAR)].texture->setRepeated(true);

    {
        //GRASS TILE
        const auto grassIndex = int(CityTileType::GRASS);
        rectCenter[grassIndex] = sf::Vector2f(tileSize) / 2.0f;
        textureRects[grassIndex] = sf::FloatRect(0, 0, tileSize.x, tileSize.y);

    }
    {
        //WATER
        auto waterIndex = int(CityTileType::WATER_VERTICAL);
        rectCenter[waterIndex] = sf::Vector2f(tileSize) / 2.0f;
        textureRects[waterIndex] = sf::FloatRect(
                7 * tileSize.x,
                5 * tileSize.y,
                tileSize.x,
                tileSize.y);
        waterIndex = int(CityTileType::WATER_HORIZONTAL);
        rectCenter[waterIndex] = sf::Vector2f(tileSize) / 2.0f;
        textureRects[waterIndex] = sf::FloatRect(
                7 * tileSize.x,
                4 * tileSize.y,
                tileSize.x,
                tileSize.y);
        waterIndex = int(CityTileType::WATER_DOWN_LEFT);
        rectCenter[waterIndex] = sf::Vector2f(tileSize) / 2.0f;
        textureRects[waterIndex] = sf::FloatRect(
                6 * tileSize.x,
                4 * tileSize.y,
                tileSize.x,
                tileSize.y);
        waterIndex = int(CityTileType::WATER_DOWN_RIGHT);
        rectCenter[waterIndex] = sf::Vector2f(tileSize) / 2.0f;
        textureRects[waterIndex] = sf::FloatRect(
                5 * tileSize.x,
                4 * tileSize.y,
                tileSize.x,
                tileSize.y);
        waterIndex = int(CityTileType::WATER_UP_LEFT);
        rectCenter[waterIndex] = sf::Vector2f(tileSize) / 2.0f;
        textureRects[waterIndex] = sf::FloatRect(
                6 * tileSize.x,
                5 * tileSize.y,
                tileSize.x,
                tileSize.y);
        waterIndex = int(CityTileType::WATER_UP_RIGHT);
        rectCenter[waterIndex] = sf::Vector2f(tileSize) / 2.0f;
        textureRects[waterIndex] = sf::FloatRect(
                5 * tileSize.x,
                5 * tileSize.y,
                tileSize.x,
                tileSize.y);
    }
    {
        //TREES
        const auto treesIndex = int(CityTileType::TREES);
        rectCenter[treesIndex] = sf::Vector2f(tileSize.x, 2.0f * tileSize.y) / 2.0f;
        textureRects[treesIndex] = sf::FloatRect(5 * tileSize.x, tileSize.y, tileSize.x, 2 * tileSize.y);
    }
    {
        //ROAD
        auto roadIndex = int(CityTileType::ROAD_LINE);
        rectCenter[roadIndex] = sf::Vector2f(tileSize.x, tileSize.y) / 2.0f;
        textureRects[roadIndex] = sf::FloatRect(tileSize.x, 0, tileSize.x, tileSize.y);

        roadIndex = int(CityTileType::ROAD_BRIDGE_HORIZONTAL);
        rectCenter[roadIndex] = sf::Vector2f(tileSize.x, 2.0f * tileSize.y) / 2.0f;
        textureRects[roadIndex] = sf::FloatRect(0, tileSize.y, tileSize.x, 2 * tileSize.y);
    }
    {
        //RAIL
        auto railIndex = int(CityTileType::RAIL_LINE);
        rectCenter[railIndex] = sf::Vector2f(tileSize.x, tileSize.y) / 2.0f;
        textureRects[railIndex] = sf::FloatRect(7.0f * tileSize.x, tileSize.y, tileSize.x, tileSize.y);

        railIndex = int(CityTileType::RAIL_TURN);
        rectCenter[railIndex] = sf::Vector2f(tileSize.x, tileSize.y) / 2.0f;
        textureRects[railIndex] = sf::FloatRect(6.0f * tileSize.x, tileSize.y, tileSize.x, tileSize.y);
    }
    {
        //TRAIN STATION
        auto trainIndex = int(CityTileType::TRAIN_STATION);
        rectCenter[trainIndex] = sf::Vector2f(5.0f * tileSize.x, 3.0f * tileSize.y) / 2.0f;
        textureRects[trainIndex] = sf::FloatRect(0.0f, 3.0f * tileSize.y, 5.0f * tileSize.x, 3.0f * tileSize.y);
    }
    for(int i = 0; i < int(CityTilesheetType::LENGTH);i++)
    {
        tilesheets[i].tilemap[0].setPrimitiveType(sf::Triangles);
        tilesheets[i].tilemap[1].setPrimitiveType(sf::Triangles);
    }

}

void CityBuilderTilemap::UpdateTilemap(CityBuilderMap& cityBuilderMap, CityTilesheetType updatedCityTileType)
{
    if(updatedCityTileType == CityTilesheetType::LENGTH)
    {
        for(unsigned i = 0u; i < unsigned(CityTilesheetType::LENGTH);i++)
        {
            UpdateTilemap(cityBuilderMap, CityTilesheetType(i));
        }
        return;
    }

    tilesheets[unsigned(updatedCityTileType)].tilemap[0].clear();
    tilesheets[unsigned(updatedCityTileType)].tilemap[1].clear();

    switch(updatedCityTileType)
    {
        //ENVIRONMENT
        case CityTilesheetType::ENVIRONMENT:
        {
            const int grassIndex = int(CityTileType::GRASS);
            //Fill the vertex array with grass
            for (auto x = 0u; x < cityBuilderMap.city.mapSize.x; x++)
            {
                for (auto y = 0u; y < cityBuilderMap.city.mapSize.y; y++)
                {

                    const auto position = sf::Vector2f(x * tileSize.x, y * tileSize.y);
                    auto& rect = textureRects[grassIndex];
                    auto& center = rectCenter[grassIndex];
                    AddNewTile(position, sf::Vector2f(tileSize), rect, center, updatedCityTileType, false, false, false);
                }
            }
            //Add water on top of it
            for (auto x = 0u; x < cityBuilderMap.city.mapSize.x; x++)
            {
                for (auto y = 0u; y < cityBuilderMap.city.mapSize.y; y++)
                {

                    int waterIndex = 0;
                    bool directions[] = {0, 0, 0, 0};//UP, DOWN, LEFT, RIGHT

                    const sf::Vector2i tmpPos = sf::Vector2i(x, y);
                    if (cityBuilderMap.environmentTiles[cityBuilderMap.Position2Index(tmpPos)] !=
                        EnvironmentTile::WATER)
                    {
                        continue;
                    }
                    //UP
                    {
                        const sf::Vector2i upPos = tmpPos + sf::Vector2i(0, -1);
                        if (upPos.y < 0 ||
                            cityBuilderMap.environmentTiles[cityBuilderMap.Position2Index(upPos)] ==
                            EnvironmentTile::WATER)
                        {
                            directions[0] = true;
                        }
                    }
                    //DOWN
                    {
                        const sf::Vector2i downPos = tmpPos + sf::Vector2i(0, 1);
                        if (downPos.y == cityBuilderMap.city.mapSize.y ||
                            cityBuilderMap.environmentTiles[cityBuilderMap.Position2Index(downPos)] ==
                            EnvironmentTile::WATER)
                        {
                            directions[1] = true;
                        }
                    }
                    //LEFT
                    {
                        const sf::Vector2i leftPos = tmpPos + sf::Vector2i(-1, 0);
                        if (leftPos.x > 0 &&
                            cityBuilderMap.environmentTiles[cityBuilderMap.Position2Index(leftPos)] ==
                            EnvironmentTile::WATER)
                        {
                            directions[2] = true;
                        }
                    }
                    //RIGHT
                    {
                        const sf::Vector2i rightPos = tmpPos + sf::Vector2i(1, 0);
                        if (rightPos.x < cityBuilderMap.city.mapSize.x &&
                            cityBuilderMap.environmentTiles[cityBuilderMap.Position2Index(rightPos)] ==
                            EnvironmentTile::WATER)
                        {
                            directions[3] = true;
                        }
                    }
                    if (directions[1] && directions[3])
                    {
                        waterIndex = int(CityTileType::WATER_DOWN_RIGHT);
                    }
                    if (directions[1] && directions[2])
                    {
                        waterIndex = int(CityTileType::WATER_DOWN_LEFT);
                    }
                    if (directions[0] && directions[3])
                    {
                        waterIndex = int(CityTileType::WATER_UP_RIGHT);
                    }
                    if (directions[0] && directions[2])
                    {
                        waterIndex = int(CityTileType::WATER_UP_LEFT);
                    }
                    if (directions[0] && directions[1])
                    {
                        waterIndex = int(CityTileType::WATER_VERTICAL);
                    }
                    if (directions[2] && directions[3])
                    {
                        waterIndex = int(CityTileType::WATER_HORIZONTAL);
                    }
                    const auto position = sf::Vector2f(x * tileSize.x, y * tileSize.y);
                    auto rect = textureRects[waterIndex];
                    auto center = rectCenter[waterIndex];
                    AddNewTile(position, sf::Vector2f(tileSize), rect, center, updatedCityTileType, false, false, false);
                }
            }
            break;
        }
        case CityTilesheetType::TRANSPORT:
        {
            //ROAD
            std::vector<CityElement> roads;
            std::copy_if(cityBuilderMap.elements.begin(), cityBuilderMap.elements.end(), std::back_inserter(roads),
                         [](CityElement& elem)
                         {
                             return elem.elementType == CityElementType::ROAD;
                         });
            for (auto& road : roads)
            {
                bool directions[] = {0, 0, 0, 0};//UP, DOWN, LEFT, RIGHT

                if (road.position.y == 0)
                {
                    directions[0] = true;
                }
                if (road.position.y == cityBuilderMap.city.mapSize.y - 1)
                {
                    directions[1] = true;
                }
                if (road.position.x == 0)
                {
                    directions[2] = true;
                }
                if (road.position.x == cityBuilderMap.city.mapSize.x - 1)
                {
                    directions[3] = true;
                }

                for (auto& otherRoad : roads)
                {
                    if (otherRoad.position == road.position)
                    {
                        continue;
                    }
                    const sf::Vector2i deltaPos = otherRoad.position - road.position;
                    if (deltaPos == sf::Vector2i(0, -1))
                    {
                        directions[0] = true;
                    }
                    if (deltaPos == sf::Vector2i(0, 1))
                    {
                        directions[1] = true;
                    }
                    if (deltaPos == sf::Vector2i(-1, 0))
                    {
                        directions[2] = true;
                    }
                    if (deltaPos == sf::Vector2i(1, 0))
                    {
                        directions[3] = true;
                    }
                }
                const auto position = sf::Vector2f(
                        road.position.x * tileSize.x,
                        road.position.y * tileSize.y);
                int roadIndex = -1;
                bool isWater = cityBuilderMap.environmentTiles[cityBuilderMap.Position2Index(road.position)] ==
                               EnvironmentTile::WATER;
                if (directions[0] && directions[1] && !directions[2] && !directions[3])
                {
                    roadIndex = int(
                            isWater ? CityTileType::ROAD_BRIDGE_VERTICAL : CityTileType::ROAD_LINE);
                    const auto rect = textureRects[roadIndex];
                    const auto center = rectCenter[roadIndex];
                    const auto size = sf::Vector2f(tileSize.x, (isWater ? 2.0f : 1.0f) * tileSize.y);

                    AddNewTile(position, size, rect, center, updatedCityTileType, false, false, false);
                }
                if (!directions[0] && !directions[1] && directions[2] && directions[3])
                {
                    roadIndex = int(
                            isWater ? CityTileType::ROAD_BRIDGE_HORIZONTAL : CityTileType::ROAD_LINE);
                    const auto rect = textureRects[roadIndex];
                    const auto center = rectCenter[roadIndex];
                    const auto size = sf::Vector2f(tileSize.x, (isWater ? 2.0f : 1.0f) * tileSize.y);

                    AddNewTile(position, size, rect, center, updatedCityTileType, false, false, !isWater);
                }
            }

            //RAILS

            std::vector<CityElement> rails;
            std::copy_if(cityBuilderMap.elements.begin(), cityBuilderMap.elements.end(), std::back_inserter(rails),
                         [](CityElement& elem)
                         {
                             return elem.elementType == CityElementType::RAIL;
                         });
            for (auto& rail : rails)
            {
                bool directions[] = {0, 0, 0, 0};//UP, DOWN, LEFT, RIGHT

                if (rail.position.y == 0)
                {
                    directions[0] = true;
                }
                if (rail.position.y == cityBuilderMap.city.mapSize.y - 1)
                {
                    directions[1] = true;
                }
                if (rail.position.x == 0)
                {
                    directions[2] = true;
                }
                if (rail.position.x == cityBuilderMap.city.mapSize.x - 1)
                {
                    directions[3] = true;
                }

                for (auto& otherRoad : rails)
                {
                    if (otherRoad.position == rail.position)
                    {
                        continue;
                    }
                    const sf::Vector2i deltaPos = otherRoad.position - rail.position;
                    if (deltaPos == sf::Vector2i(0, -1))
                    {
                        directions[0] = true;
                    }
                    if (deltaPos == sf::Vector2i(0, 1))
                    {
                        directions[1] = true;
                    }
                    if (deltaPos == sf::Vector2i(-1, 0))
                    {
                        directions[2] = true;
                    }
                    if (deltaPos == sf::Vector2i(1, 0))
                    {
                        directions[3] = true;
                    }
                }
                const auto position = sf::Vector2f(
                        rail.position.x * tileSize.x,
                        rail.position.y * tileSize.y);
                //HORIZONTAL FIRST
                if (directions[2] && directions[3])
                {
                    const int railIndex = int(CityTileType::RAIL_LINE);
                    const auto rect = textureRects[railIndex];
                    const auto center = rectCenter[railIndex];
                    const auto size = sf::Vector2f(tileSize);

                    AddNewTile(position, size, rect, center, updatedCityTileType, false, false, false);
                }

                    //UP RIGHT
                else if (directions[0] && directions[3])
                {
                    const int railIndex = int(CityTileType::RAIL_TURN);
                    const auto rect = textureRects[railIndex];
                    const auto center = rectCenter[railIndex];
                    const auto size = sf::Vector2f(tileSize);

                    AddNewTile(position, size, rect, center, updatedCityTileType, false, false, false);
                }
                    //DOWN LEFT
                else if (directions[1] && directions[2])
                {
                    const int railIndex = int(CityTileType::RAIL_TURN);
                    const auto rect = textureRects[railIndex];
                    const auto center = rectCenter[railIndex];
                    const auto size = sf::Vector2f(tileSize);

                    AddNewTile(position, size, rect, center, updatedCityTileType, true, true, false);
                }
                    //DOWN RIGHT
                else if (directions[1] && directions[3])
                {
                    const int railIndex = int(CityTileType::RAIL_TURN);
                    const auto rect = textureRects[railIndex];
                    const auto center = rectCenter[railIndex];
                    const auto size = sf::Vector2f(tileSize);

                    AddNewTile(position, size, rect, center, updatedCityTileType, false, true, false);
                }
                    //UP LEFT
                else if (directions[0] && directions[2])
                {
                    const int railIndex = int(CityTileType::RAIL_TURN);
                    const auto rect = textureRects[railIndex];
                    const auto center = rectCenter[railIndex];
                    const auto size = sf::Vector2f(tileSize);

                    AddNewTile(position, size, rect, center, updatedCityTileType, true, false, false);
                }
                    //VERTICAL LAST
                else if (directions[0] && directions[1])
                {
                    const int railIndex = int(CityTileType::RAIL_LINE);
                    const auto rect = textureRects[railIndex];
                    const auto center = rectCenter[railIndex];
                    const auto size = sf::Vector2f(tileSize);

                    AddNewTile(position, size, rect, center, updatedCityTileType, false, false, true);
                }
            }
            break;
        }

        case CityTilesheetType::CITY:
        {
            //TREES
            const int treesIndex = int(CityTileType::TREES);

            for (auto& element : cityBuilderMap.elements)
            {
                switch (element.elementType)
                {
                    case CityElementType::TREES:
                    {
                        if (cityBuilderMap.environmentTiles[cityBuilderMap.Position2Index(element.position)] ==
                            EnvironmentTile::WATER)
                        {
                            continue;
                        }
                        const auto position = sf::Vector2f(
                                element.position.x * tileSize.x,
                                element.position.y * tileSize.y);
                        const auto rect = textureRects[treesIndex];
                        const auto center = rectCenter[treesIndex];
                        const auto size = sf::Vector2f(
                                tileSize.x,
                                2.0f * tileSize.y
                        );

                        AddNewTile(position, size, rect, center, updatedCityTileType, false, false, false);
                        break;
                    }
                    case CityElementType::TRAIN_STATION:
                    {
                        const int trainStationIndex = int(CityTileType::TRAIN_STATION);
                        const auto position = sf::Vector2f(
                                element.position.x * tileSize.x,
                                element.position.y * tileSize.y);
                        const auto rect = textureRects[trainStationIndex];
                        const auto center = rectCenter[trainStationIndex];
                        const auto size = sf::Vector2f(
                                element.size.x * tileSize.x,
                                element.size.y * tileSize.y
                        );
                        AddNewTile(position, size, rect, center, updatedCityTileType, false, false, false);
                        break;
                    }
                    default:
                        break;
                }
            }
            break;
        }
        default:
            break;
    }

}

void CityBuilderTilemap::PushCommand(GraphicsManager* graphicsManager)
{
    rmt_ScopedCPUSample(PushCityTilemapCommands, 0);
    const int frameIndex = MainEngine::GetInstance()->frameIndex % 2;
    graphicsManager->Draw(&tilesheets[unsigned(CityTilesheetType::ENVIRONMENT)].tilemap[frameIndex], tilesheets[unsigned(CityTilesheetType::ENVIRONMENT)].texture.get());
    graphicsManager->Draw(&tilesheets[unsigned(CityTilesheetType::TRANSPORT)].tilemap[frameIndex], tilesheets[unsigned(CityTilesheetType::TRANSPORT)].texture.get());
    graphicsManager->Draw(&tilesheets[unsigned(CityTilesheetType::CAR)].tilemap[frameIndex], tilesheets[unsigned(CityTilesheetType::CAR)].texture.get());
    graphicsManager->Draw(&tilesheets[unsigned(CityTilesheetType::CITY)].tilemap[frameIndex], tilesheets[unsigned(CityTilesheetType::CITY)].texture.get());

}

void CityBuilderTilemap::AddNewTile(const sf::Vector2f position, const sf::Vector2f size, const sf::FloatRect rect,
                                    const sf::Vector2f center, CityTilesheetType updatedCityTileType, bool flipX,
                                    bool flipY, bool rotate90)
{
    sf::Vector2f sizeOffset = (size - sf::Vector2f(tileSize)) / 2.0f;
    sizeOffset.x = -sizeOffset.x;
    sf::Vertex quad[6];
    quad[0].position = position - center - sizeOffset;
    quad[1].position = position - sf::Vector2f(center.x, -center.y) - sizeOffset;
    quad[2].position = position + center - sizeOffset;
    quad[3].position = position - center - sizeOffset;
    quad[4].position = position + center - sizeOffset;
    quad[5].position = position - sf::Vector2f(-center.x, center.y) - sizeOffset;

    if (rotate90)
    {
        quad[0].texCoords = sf::Vector2f(//rect.left + rect.width,rect.top
                rect.left + rect.width, rect.top);
        quad[1].texCoords = sf::Vector2f(//rect.left,rect.top
                rect.left, rect.top);
        quad[2].texCoords = sf::Vector2f(//rect.left,rect.top + rect.height
                rect.left, rect.top + rect.height);
        quad[3].texCoords = sf::Vector2f(//rect.left + rect.width,rect.top
                rect.left + rect.width, rect.top);
        quad[4].texCoords = sf::Vector2f(//rect.left,rect.top + rect.height
                rect.left, rect.top + rect.height);
        quad[5].texCoords = sf::Vector2f(//rect.left + rect.width,rect.top + rect.height
                rect.left + rect.width, rect.top + rect.height);
    }
    else
    {
        quad[0].texCoords = sf::Vector2f(
                flipX ? rect.left + rect.width : rect.left,
                flipY ? rect.top + rect.height : rect.top);
        quad[1].texCoords = sf::Vector2f(
                flipX ? rect.left + rect.width : rect.left,
                flipY ? rect.top : rect.top + rect.height);
        quad[2].texCoords = sf::Vector2f(
                flipX ? rect.left : rect.left + rect.width,
                flipY ? rect.top : rect.top + rect.height);
        quad[3].texCoords = sf::Vector2f(
                flipX ? rect.left + rect.width : rect.left,
                flipY ? rect.top + rect.height : rect.top);
        quad[4].texCoords = sf::Vector2f(
                flipX ? rect.left : rect.left + rect.width,
                flipY ? rect.top : rect.top + rect.height);
        quad[5].texCoords = sf::Vector2f(
                flipX ? rect.left : rect.left + rect.width,
                flipY ? rect.top + rect.height : rect.top);
    }
    for (auto& v : quad)
    {
        tilesheets[unsigned(updatedCityTileType)].tilemap[0].append(v);
        tilesheets[unsigned(updatedCityTileType)].tilemap[1].append(v);
    }
}
}
