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

    tilesheet.texture = textureManager.LoadTexture(textureName);
    tilesheet.texture->setRepeated(true);

    {
        //GRASS TILE
        const auto grassIndex = int(CityBuilderTileType::GRASS);
        tilesheet.center[grassIndex] = sf::Vector2f(tileSize) / 2.0f;
        tilesheet.rect[grassIndex] = sf::FloatRect(0, 0, tileSize.x, tileSize.y);

    }
    {
        //WATER
        auto waterIndex = int(CityBuilderTileType::WATER_VERTICAL);
        tilesheet.center[waterIndex] = sf::Vector2f(tileSize) / 2.0f;
        tilesheet.rect[waterIndex] = sf::FloatRect(
                7 * tileSize.x,
                5 * tileSize.y,
                tileSize.x,
                tileSize.y);
        waterIndex = int(CityBuilderTileType::WATER_HORIZONTAL);
        tilesheet.center[waterIndex] = sf::Vector2f(tileSize) / 2.0f;
        tilesheet.rect[waterIndex] = sf::FloatRect(
                7 * tileSize.x,
                4 * tileSize.y,
                tileSize.x,
                tileSize.y);
        waterIndex = int(CityBuilderTileType::WATER_DOWN_LEFT);
        tilesheet.center[waterIndex] = sf::Vector2f(tileSize) / 2.0f;
        tilesheet.rect[waterIndex] = sf::FloatRect(
                6 * tileSize.x,
                4 * tileSize.y,
                tileSize.x,
                tileSize.y);
        waterIndex = int(CityBuilderTileType::WATER_DOWN_RIGHT);
        tilesheet.center[waterIndex] = sf::Vector2f(tileSize) / 2.0f;
        tilesheet.rect[waterIndex] = sf::FloatRect(
                5 * tileSize.x,
                4 * tileSize.y,
                tileSize.x,
                tileSize.y);
        waterIndex = int(CityBuilderTileType::WATER_UP_LEFT);
        tilesheet.center[waterIndex] = sf::Vector2f(tileSize) / 2.0f;
        tilesheet.rect[waterIndex] = sf::FloatRect(
                6 * tileSize.x,
                5 * tileSize.y,
                tileSize.x,
                tileSize.y);
        waterIndex = int(CityBuilderTileType::WATER_UP_RIGHT);
        tilesheet.center[waterIndex] = sf::Vector2f(tileSize) / 2.0f;
        tilesheet.rect[waterIndex] = sf::FloatRect(
                5 * tileSize.x,
                5 * tileSize.y,
                tileSize.x,
                tileSize.y);
    }
    {
        //TREES
        const auto treesIndex = int(CityBuilderTileType::TREES);
        tilesheet.center[treesIndex] = sf::Vector2f(tileSize.x, 2.0f * tileSize.y) / 2.0f;
        tilesheet.rect[treesIndex] = sf::FloatRect(5 * tileSize.x, tileSize.y, tileSize.x, 2 * tileSize.y);
    }
    {
        //ROAD
        auto roadIndex = int(CityBuilderTileType::ROAD_LINE);
        tilesheet.center[roadIndex] = sf::Vector2f(tileSize.x, tileSize.y) / 2.0f;
        tilesheet.rect[roadIndex] = sf::FloatRect(tileSize.x, 0, tileSize.x, tileSize.y);

        roadIndex = int(CityBuilderTileType::ROAD_BRIDGE_HORIZONTAL);
        tilesheet.center[roadIndex] = sf::Vector2f(tileSize.x, 2.0f * tileSize.y) / 2.0f;
        tilesheet.rect[roadIndex] = sf::FloatRect(0, tileSize.y, tileSize.x, 2 * tileSize.y);
    }
    {
        //RAIL
        auto railIndex = int(CityBuilderTileType::RAIL_LINE);
        tilesheet.center[railIndex] = sf::Vector2f(tileSize.x, tileSize.y) / 2.0f;
        tilesheet.rect[railIndex] = sf::FloatRect(7.0f * tileSize.x, tileSize.y, tileSize.x, tileSize.y);

        railIndex = int(CityBuilderTileType::RAIL_TURN);
        tilesheet.center[railIndex] = sf::Vector2f(tileSize.x, tileSize.y) / 2.0f;
        tilesheet.rect[railIndex] = sf::FloatRect(6.0f * tileSize.x, tileSize.y, tileSize.x, tileSize.y);
    }
    {
        //TRAIN STATION
        auto trainIndex = int(CityBuilderTileType::TRAIN_STATION);
        tilesheet.center[trainIndex] = sf::Vector2f(5.0f * tileSize.x, 3.0f * tileSize.y) / 2.0f;
        tilesheet.rect[trainIndex] = sf::FloatRect(0.0f, 3.0f * tileSize.y, 5.0f * tileSize.x, 3.0f * tileSize.y);
    }
    tilesheet.tilemap[0].setPrimitiveType(sf::Triangles);
    tilesheet.tilemap[1].setPrimitiveType(sf::Triangles);
}

void CityBuilderTilemap::UpdateTilemap(CityBuilderMap& cityBuilderMap)
{
    tilesheet.tilemap[0].clear();
    tilesheet.tilemap[1].clear();
    //ENVIRONMENT
    {
        const int grassIndex = int(CityBuilderTileType::GRASS);
        //Fill the vertex array with grass
        for (auto x = 0u; x < cityBuilderMap.city.mapSize.x; x++)
        {
            for (auto y = 0u; y < cityBuilderMap.city.mapSize.y; y++)
            {

                const auto position = sf::Vector2f(x * tileSize.x, y * tileSize.y);
                auto rect = tilesheet.rect[grassIndex];
                auto center = tilesheet.center[grassIndex];
                AddNewTile(position, sf::Vector2f(tileSize), rect, center);
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
                if (cityBuilderMap.environmentTiles[cityBuilderMap.Position2Index(tmpPos)] != EnvironmentTile::WATER)
                {
                    continue;
                }
                //UP
                {
                    const sf::Vector2i upPos = tmpPos + sf::Vector2i(0, -1);
                    if (upPos.y < 0 ||
                        cityBuilderMap.environmentTiles[cityBuilderMap.Position2Index(upPos)] == EnvironmentTile::WATER)
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
                    waterIndex = int(CityBuilderTileType::WATER_DOWN_RIGHT);
                }
                if (directions[1] && directions[2])
                {
                    waterIndex = int(CityBuilderTileType::WATER_DOWN_LEFT);
                }
                if (directions[0] && directions[3])
                {
                    waterIndex = int(CityBuilderTileType::WATER_UP_RIGHT);
                }
                if (directions[0] && directions[2])
                {
                    waterIndex = int(CityBuilderTileType::WATER_UP_LEFT);
                }
                if (directions[0] && directions[1])
                {
                    waterIndex = int(CityBuilderTileType::WATER_VERTICAL);
                }
                if (directions[2] && directions[3])
                {
                    waterIndex = int(CityBuilderTileType::WATER_HORIZONTAL);
                }
                const auto position = sf::Vector2f(x * tileSize.x, y * tileSize.y);
                auto rect = tilesheet.rect[waterIndex];
                auto center = tilesheet.center[waterIndex];
                AddNewTile(position, sf::Vector2f(tileSize), rect, center);
            }
        }
    }
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
                roadIndex = int(isWater ? CityBuilderTileType::ROAD_BRIDGE_VERTICAL : CityBuilderTileType::ROAD_LINE);
                const auto rect = tilesheet.rect[roadIndex];
                const auto center = tilesheet.center[roadIndex];
                const auto size = sf::Vector2f(tileSize.x, (isWater ? 2.0f : 1.0f) * tileSize.y);

                AddNewTile(position, size, rect, center);
            }
            if (!directions[0] && !directions[1] && directions[2] && directions[3])
            {
                roadIndex = int(isWater ? CityBuilderTileType::ROAD_BRIDGE_HORIZONTAL : CityBuilderTileType::ROAD_LINE);
                const auto rect = tilesheet.rect[roadIndex];
                const auto center = tilesheet.center[roadIndex];
                const auto size = sf::Vector2f(tileSize.x, (isWater ? 2.0f : 1.0f) * tileSize.y);

                AddNewTile(position, size, rect, center, false, false, !isWater);
            }
        }
    }

    {
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
                const int railIndex = int(CityBuilderTileType::RAIL_LINE);
                const auto rect = tilesheet.rect[railIndex];
                const auto center = tilesheet.center[railIndex];
                const auto size = sf::Vector2f(tileSize);

                AddNewTile(position, size, rect, center);
            }

                //UP RIGHT
            else if (directions[0] && directions[3])
            {
                const int railIndex = int(CityBuilderTileType::RAIL_TURN);
                const auto rect = tilesheet.rect[railIndex];
                const auto center = tilesheet.center[railIndex];
                const auto size = sf::Vector2f(tileSize);

                AddNewTile(position, size, rect, center);
            }
                //DOWN LEFT
            else if (directions[1] && directions[2])
            {
                const int railIndex = int(CityBuilderTileType::RAIL_TURN);
                const auto rect = tilesheet.rect[railIndex];
                const auto center = tilesheet.center[railIndex];
                const auto size = sf::Vector2f(tileSize);

                AddNewTile(position, size, rect, center, true, true);
            }
                //DOWN RIGHT
            else if (directions[1] && directions[3])
            {
                const int railIndex = int(CityBuilderTileType::RAIL_TURN);
                const auto rect = tilesheet.rect[railIndex];
                const auto center = tilesheet.center[railIndex];
                const auto size = sf::Vector2f(tileSize);

                AddNewTile(position, size, rect, center, false, true);
            }
                //UP LEFT
            else if (directions[0] && directions[2])
            {
                const int railIndex = int(CityBuilderTileType::RAIL_TURN);
                const auto rect = tilesheet.rect[railIndex];
                const auto center = tilesheet.center[railIndex];
                const auto size = sf::Vector2f(tileSize);

                AddNewTile(position, size, rect, center, true);
            }
                //VERTICAL LAST
            else if (directions[0] && directions[1])
            {
                const int railIndex = int(CityBuilderTileType::RAIL_LINE);
                const auto rect = tilesheet.rect[railIndex];
                const auto center = tilesheet.center[railIndex];
                const auto size = sf::Vector2f(tileSize);

                AddNewTile(position, size, rect, center, false, false, true);
            }
        }
    }


    {
        //TREES
        const int treesIndex = int(CityBuilderTileType::TREES);

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
                    const auto rect = tilesheet.rect[treesIndex];
                    const auto center = tilesheet.center[treesIndex];
                    const auto size = sf::Vector2f(
                            tileSize.x,
                            2.0f * tileSize.y
                    );

                    AddNewTile(position, size, rect, center);
                    break;
                }
                case CityElementType::TRAIN_STATION:
                {
                    const int trainStationIndex = int(CityBuilderTileType::TRAIN_STATION);
                    const auto position = sf::Vector2f(
                            element.position.x * tileSize.x,
                            element.position.y * tileSize.y);
                    const auto rect = tilesheet.rect[trainStationIndex];
                    const auto center = tilesheet.center[trainStationIndex];
                    const auto size = sf::Vector2f(
                            element.size.x * tileSize.x,
                            element.size.y * tileSize.y
                    );
                    AddNewTile(position, size, rect, center);
                    break;
                }
                default:
                    break;
            }
        }
    }

}

void CityBuilderTilemap::PushCommand(GraphicsManager* graphicsManager)
{
    rmt_ScopedCPUSample(PushCityTilemapCommands, 0);
    const int frameIndex = MainEngine::GetInstance()->frameIndex % 2;
    graphicsManager->Draw(&tilesheet.tilemap[frameIndex], tilesheet.texture);

}

void CityBuilderTilemap::AddNewTile(
        const sf::Vector2f position,
        const sf::Vector2f size,
        const sf::FloatRect rect,
        const sf::Vector2f center,
        bool flipX,
        bool flipY,
        bool rotate90)
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
        tilesheet.tilemap[0].append(v);
        tilesheet.tilemap[1].append(v);
    }
}
}
