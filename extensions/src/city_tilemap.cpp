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
    for (int i = 0; i < int(CityTilesheetType::CAR); i++)
    {
        Index textureIndex = textureManager.LoadTexture(cityTextureName_);
        tilesheets_[i].texture = textureManager.GetTexture(textureIndex);
        tilesheets_[i].texture->setRepeated(true);
    }
    const Index textureIndex = textureManager.LoadTexture(carTextureName_);
    tilesheets_[unsigned(CityTilesheetType::CAR)].texture = textureManager.GetTexture(textureIndex);
    tilesheets_[unsigned(CityTilesheetType::CAR)].texture->setRepeated(true);

    {
        //GRASS TILE
        const auto grassIndex = int(CityTileType::GRASS);
        rectCenter_[grassIndex] = sf::Vector2f(tileSize_) / 2.0f;
        textureRects_[grassIndex] = sf::FloatRect(0, 0, tileSize_.x, tileSize_.y);

    }
    {
        //WATER
        auto waterIndex = int(CityTileType::WATER_VERTICAL);
        rectCenter_[waterIndex] = sf::Vector2f(tileSize_) / 2.0f;
        textureRects_[waterIndex] = sf::FloatRect(
                7 * tileSize_.x,
                5 * tileSize_.y,
                tileSize_.x,
                tileSize_.y);
        waterIndex = int(CityTileType::WATER_HORIZONTAL);
        rectCenter_[waterIndex] = sf::Vector2f(tileSize_) / 2.0f;
        textureRects_[waterIndex] = sf::FloatRect(
                7 * tileSize_.x,
                4 * tileSize_.y,
                tileSize_.x,
                tileSize_.y);
        waterIndex = int(CityTileType::WATER_DOWN_LEFT);
        rectCenter_[waterIndex] = sf::Vector2f(tileSize_) / 2.0f;
        textureRects_[waterIndex] = sf::FloatRect(
                6 * tileSize_.x,
                4 * tileSize_.y,
                tileSize_.x,
                tileSize_.y);
        waterIndex = int(CityTileType::WATER_DOWN_RIGHT);
        rectCenter_[waterIndex] = sf::Vector2f(tileSize_) / 2.0f;
        textureRects_[waterIndex] = sf::FloatRect(
                5 * tileSize_.x,
                4 * tileSize_.y,
                tileSize_.x,
                tileSize_.y);
        waterIndex = int(CityTileType::WATER_UP_LEFT);
        rectCenter_[waterIndex] = sf::Vector2f(tileSize_) / 2.0f;
        textureRects_[waterIndex] = sf::FloatRect(
                6 * tileSize_.x,
                5 * tileSize_.y,
                tileSize_.x,
                tileSize_.y);
        waterIndex = int(CityTileType::WATER_UP_RIGHT);
        rectCenter_[waterIndex] = sf::Vector2f(tileSize_) / 2.0f;
        textureRects_[waterIndex] = sf::FloatRect(
                5 * tileSize_.x,
                5 * tileSize_.y,
                tileSize_.x,
                tileSize_.y);
    }
    {
        //TREES
        const auto treesIndex = int(CityTileType::TREES);
        rectCenter_[treesIndex] = sf::Vector2f(tileSize_.x, 2.0f * tileSize_.y) / 2.0f;
        textureRects_[treesIndex] = sf::FloatRect(5 * tileSize_.x, tileSize_.y, tileSize_.x, 2 * tileSize_.y);
    }
    {
        //ROAD
        auto roadIndex = int(CityTileType::ROAD_LINE);
        rectCenter_[roadIndex] = sf::Vector2f(tileSize_.x, tileSize_.y) / 2.0f;
        textureRects_[roadIndex] = sf::FloatRect(tileSize_.x, 0, tileSize_.x, tileSize_.y);

        roadIndex = int(CityTileType::ROAD_BRIDGE_HORIZONTAL);
        rectCenter_[roadIndex] = sf::Vector2f(tileSize_.x, 2.0f * tileSize_.y) / 2.0f;
        textureRects_[roadIndex] = sf::FloatRect(0, tileSize_.y, tileSize_.x, 2 * tileSize_.y);
    }
    {
        //RAIL
        auto railIndex = int(CityTileType::RAIL_LINE);
        rectCenter_[railIndex] = sf::Vector2f(tileSize_.x, tileSize_.y) / 2.0f;
        textureRects_[railIndex] = sf::FloatRect(7.0f * tileSize_.x, tileSize_.y, tileSize_.x, tileSize_.y);

        railIndex = int(CityTileType::RAIL_TURN);
        rectCenter_[railIndex] = sf::Vector2f(tileSize_.x, tileSize_.y) / 2.0f;
        textureRects_[railIndex] = sf::FloatRect(6.0f * tileSize_.x, tileSize_.y, tileSize_.x, tileSize_.y);
    }
    {
        //TRAIN STATION
        auto trainIndex = int(CityTileType::TRAIN_STATION);
        rectCenter_[trainIndex] = sf::Vector2f(5.0f * tileSize_.x, 3.0f * tileSize_.y) / 2.0f;
        textureRects_[trainIndex] = sf::FloatRect(0.0f, 3.0f * tileSize_.y, 5.0f * tileSize_.x, 3.0f * tileSize_.y);
    }
    for (int i = 0; i < int(CityTilesheetType::LENGTH); i++)
    {
        tilesheets_[i].tilemap[0].setPrimitiveType(sf::Triangles);
        tilesheets_[i].tilemap[1].setPrimitiveType(sf::Triangles);
    }

}

void CityBuilderTilemap::UpdateTilemap(CityBuilderMap& cityBuilderMap, sf::View mainView,
                                       CityTilesheetType updatedCityTileType)
{
    const Index frameIndex = MainEngine::GetInstance()->frameIndex % 2;
    //Manage window view
    windowView_ = sf::FloatRect((mainView.getCenter() - mainView.getSize() / 2.0f), mainView.getSize());

    if (updatedCityTileType == CityTilesheetType::LENGTH)
    {
        rmt_ScopedCPUSample(UpdateTilemap, 0);
        for (unsigned i = 0u; i < unsigned(CityTilesheetType::LENGTH); i++)
        {
            UpdateTilemap(cityBuilderMap, mainView, CityTilesheetType(i));
        }
        return;
    }
    rmt_ScopedCPUSample(UpdateIndividualTilemap, 0);
    tilesheets_[unsigned(updatedCityTileType)].tilemap[frameIndex].clear();

    switch (updatedCityTileType)
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

                    const auto position = sf::Vector2f(x * tileSize_.x, y * tileSize_.y);
                    auto& rect = textureRects_[grassIndex];
                    auto& center = rectCenter_[grassIndex];
                    AddNewTile(position, sf::Vector2f(tileSize_), rect, center, updatedCityTileType);
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
                    if (cityBuilderMap.environmentTiles_[cityBuilderMap.Position2Index(tmpPos)] !=
                        EnvironmentTile::WATER)
                    {
                        continue;
                    }
                    //UP
                    {
                        const sf::Vector2i upPos = tmpPos + sf::Vector2i(0, -1);
                        if (upPos.y < 0 ||
                            cityBuilderMap.environmentTiles_[cityBuilderMap.Position2Index(upPos)] ==
                            EnvironmentTile::WATER)
                        {
                            directions[0] = true;
                        }
                    }
                    //DOWN
                    {
                        const sf::Vector2i downPos = tmpPos + sf::Vector2i(0, 1);
                        if (downPos.y == cityBuilderMap.city.mapSize.y ||
                            cityBuilderMap.environmentTiles_[cityBuilderMap.Position2Index(downPos)] ==
                            EnvironmentTile::WATER)
                        {
                            directions[1] = true;
                        }
                    }
                    //LEFT
                    {
                        const sf::Vector2i leftPos = tmpPos + sf::Vector2i(-1, 0);
                        if (leftPos.x > 0 &&
                            cityBuilderMap.environmentTiles_[cityBuilderMap.Position2Index(leftPos)] ==
                            EnvironmentTile::WATER)
                        {
                            directions[2] = true;
                        }
                    }
                    //RIGHT
                    {
                        const sf::Vector2i rightPos = tmpPos + sf::Vector2i(1, 0);
                        if (rightPos.x < cityBuilderMap.city.mapSize.x &&
                            cityBuilderMap.environmentTiles_[cityBuilderMap.Position2Index(rightPos)] ==
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
                    const auto position = sf::Vector2f(x * tileSize_.x, y * tileSize_.y);
                    auto rect = textureRects_[waterIndex];
                    auto center = rectCenter_[waterIndex];
                    AddNewTile(position, sf::Vector2f(tileSize_), rect, center, updatedCityTileType);
                }
            }
            break;
        }
        case CityTilesheetType::TRANSPORT:
        {
            //ROAD
            std::vector<CityElement> roads;
            std::copy_if(cityBuilderMap.elements_.begin(), cityBuilderMap.elements_.end(), std::back_inserter(roads),
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
                        road.position.x * tileSize_.x,
                        road.position.y * tileSize_.y);
                int roadIndex = -1;
                bool isWater = cityBuilderMap.environmentTiles_[cityBuilderMap.Position2Index(road.position)] ==
                               EnvironmentTile::WATER;
                if (directions[0] && directions[1] && !directions[2] && !directions[3])
                {
                    roadIndex = int(
                            isWater ? CityTileType::ROAD_BRIDGE_VERTICAL : CityTileType::ROAD_LINE);
                    const auto rect = textureRects_[roadIndex];
                    const auto center = rectCenter_[roadIndex];
                    const auto size = sf::Vector2f(tileSize_.x, (isWater ? 2.0f : 1.0f) * tileSize_.y);

                    AddNewTile(position, size, rect, center, updatedCityTileType);
                }
                if (!directions[0] && !directions[1] && directions[2] && directions[3])
                {
                    roadIndex = int(
                            isWater ? CityTileType::ROAD_BRIDGE_HORIZONTAL : CityTileType::ROAD_LINE);
                    const auto rect = textureRects_[roadIndex];
                    const auto center = rectCenter_[roadIndex];
                    const auto size = sf::Vector2f(tileSize_.x, (isWater ? 2.0f : 1.0f) * tileSize_.y);

                    AddNewTile(position, size, rect, center, updatedCityTileType, false, false, !isWater, true);
                }
            }

            //RAILS

            std::vector<CityElement> rails;
            std::copy_if(cityBuilderMap.elements_.begin(), cityBuilderMap.elements_.end(), std::back_inserter(rails),
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
                        rail.position.x * tileSize_.x,
                        rail.position.y * tileSize_.y);
                //HORIZONTAL FIRST
                if (directions[2] && directions[3])
                {
                    const int railIndex = int(CityTileType::RAIL_LINE);
                    const auto rect = textureRects_[railIndex];
                    const auto center = rectCenter_[railIndex];
                    const auto size = sf::Vector2f(tileSize_);

                    AddNewTile(position, size, rect, center, updatedCityTileType);
                }

                    //UP RIGHT
                else if (directions[0] && directions[3])
                {
                    const int railIndex = int(CityTileType::RAIL_TURN);
                    const auto rect = textureRects_[railIndex];
                    const auto center = rectCenter_[railIndex];
                    const auto size = sf::Vector2f(tileSize_);

                    AddNewTile(position, size, rect, center, updatedCityTileType);
                }
                    //DOWN LEFT
                else if (directions[1] && directions[2])
                {
                    const int railIndex = int(CityTileType::RAIL_TURN);
                    const auto rect = textureRects_[railIndex];
                    const auto center = rectCenter_[railIndex];
                    const auto size = sf::Vector2f(tileSize_);

                    AddNewTile(position, size, rect, center, updatedCityTileType, true, true, false, true);
                }
                    //DOWN RIGHT
                else if (directions[1] && directions[3])
                {
                    const int railIndex = int(CityTileType::RAIL_TURN);
                    const auto rect = textureRects_[railIndex];
                    const auto center = rectCenter_[railIndex];
                    const auto size = sf::Vector2f(tileSize_);

                    AddNewTile(position, size, rect, center, updatedCityTileType, false, true, false, true);
                }
                    //UP LEFT
                else if (directions[0] && directions[2])
                {
                    const int railIndex = int(CityTileType::RAIL_TURN);
                    const auto rect = textureRects_[railIndex];
                    const auto center = rectCenter_[railIndex];
                    const auto size = sf::Vector2f(tileSize_);

                    AddNewTile(position, size, rect, center, updatedCityTileType, true, false, false, true);
                }
                    //VERTICAL LAST
                else if (directions[0] && directions[1])
                {
                    const int railIndex = int(CityTileType::RAIL_LINE);
                    const auto rect = textureRects_[railIndex];
                    const auto center = rectCenter_[railIndex];
                    const auto size = sf::Vector2f(tileSize_);

                    AddNewTile(position, size, rect, center, updatedCityTileType, false, false, true, true);
                }
            }
            break;
        }

        case CityTilesheetType::CITY:
        {
            //TREES
            const int treesIndex = int(CityTileType::TREES);

            for (auto& element : cityBuilderMap.elements_)
            {
                switch (element.elementType)
                {
                    case CityElementType::TREES:
                    {
                        if (cityBuilderMap.environmentTiles_[cityBuilderMap.Position2Index(element.position)] ==
                            EnvironmentTile::WATER)
                        {
                            continue;
                        }
                        const auto position = sf::Vector2f(
                                element.position.x * tileSize_.x,
                                element.position.y * tileSize_.y);
                        const auto rect = textureRects_[treesIndex];
                        const auto center = rectCenter_[treesIndex];
                        const auto size = sf::Vector2f(
                                tileSize_.x,
                                2.0f * tileSize_.y
                        );

                        AddNewTile(position, size, rect, center, updatedCityTileType);
                        break;
                    }
                    case CityElementType::TRAIN_STATION:
                    {
                        const int trainStationIndex = int(CityTileType::TRAIN_STATION);
                        const auto position = sf::Vector2f(
                                element.position.x * tileSize_.x,
                                element.position.y * tileSize_.y);
                        const auto rect = textureRects_[trainStationIndex];
                        const auto center = rectCenter_[trainStationIndex];
                        const auto size = sf::Vector2f(
                                element.size.x * tileSize_.x,
                                element.size.y * tileSize_.y
                        );
                        AddNewTile(position, size, rect, center, updatedCityTileType);
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
    graphicsManager->Draw(&tilesheets_[unsigned(CityTilesheetType::ENVIRONMENT)].tilemap[frameIndex],
                          tilesheets_[unsigned(CityTilesheetType::ENVIRONMENT)].texture.get());
    graphicsManager->Draw(&tilesheets_[unsigned(CityTilesheetType::TRANSPORT)].tilemap[frameIndex],
                          tilesheets_[unsigned(CityTilesheetType::TRANSPORT)].texture.get());
    graphicsManager->Draw(&tilesheets_[unsigned(CityTilesheetType::CAR)].tilemap[frameIndex],
                          tilesheets_[unsigned(CityTilesheetType::CAR)].texture.get());
    graphicsManager->Draw(&tilesheets_[unsigned(CityTilesheetType::CITY)].tilemap[frameIndex],
                          tilesheets_[unsigned(CityTilesheetType::CITY)].texture.get());

}

void CityBuilderTilemap::AddNewTile(const sf::Vector2f position, const sf::Vector2f size, const sf::FloatRect rect,
                                    const sf::Vector2f center, CityTilesheetType updatedCityTileType, bool flipX,
                                    bool flipY, bool rotate90, bool culling)
{
    const Index frameIndex = MainEngine::GetInstance()->frameIndex % 2;
    sf::Vector2f sizeOffset = (size - sf::Vector2f(tileSize_)) / 2.0f;
    sizeOffset.x = -sizeOffset.x;
    if (culling)
    {

        sf::FloatRect tileRect = sf::FloatRect((position - center - sizeOffset), size);

        if (!windowView_.intersects(tileRect))
            return;
    }
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
        tilesheets_[unsigned(updatedCityTileType)].tilemap[frameIndex].append(v);
    }
}
}
