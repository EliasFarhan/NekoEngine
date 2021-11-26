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
#include <utilities/time_utility.h>
#include "City/city_map.h"
#include "City/city_tilemap.h"
#include "City/city_zone.h"

namespace neko
{

struct Building
{
    sf::Vector2i position = INVALID_TILE_POS;
    sf::Vector2i size = sf::Vector2i(1,1);
    CityTileType buildingType = CityTileType::NONE;
    ZoneType zoneType = ZoneType::NONE;
	unsigned capacity = 0;
	unsigned occupancy = 0;
};

class CityBuildingManager
{
public:
    void Update(CityZoneManager& zoneManager, CityBuilderMap& cityMap, const float dt);
    void AddBuilding(Building building, CityZoneManager& zoneManager, CityBuilderMap& cityMap);
    void RemoveBuilding(sf::Vector2i position);
	sf::Vector2i FindBuilding(ZoneType zoneType);
    std::pair<Building*, std::shared_lock<std::shared_mutex>> GetBuildingAt(sf::Vector2i position);
    const std::vector<Building>& GetBuildingsVector() const;
private:
    mutable std::shared_mutex buildingMutex;
    std::vector<Building> buildings_;
    Timer spawnTimer_ = Timer(0.0f, 0.2f);
};
}