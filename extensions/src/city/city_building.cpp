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

#include <city/city_building.h>
#include <engine/engine.h>

namespace neko
{

void CityBuildingManager::Update(const CityZoneManager& zoneManager, CityBuilderMap& cityMap)
{
    auto* engine = MainEngine::GetInstance();
    spawnTimer_.Update(engine->dt.asSeconds());
    if (spawnTimer_.IsOver())
    {
        //Add residential building
        {
            const auto& zones = zoneManager.GetZoneVector();
            std::vector<Zone> residentialZones;
            residentialZones.reserve(zones.size());
            std::copy_if(zones.begin(), zones.end(), std::back_inserter(residentialZones), [&](const Zone& zone)
            {
                const auto buildingAtPos = std::find_if(buildings_.begin(), buildings_.end(),[&zone](const Building& building){
                    return building.position == zone.position;
                });
                return zone.zoneType == ZoneType::RESIDENTIAL && buildingAtPos == buildings_.end();
            });
            if (!residentialZones.empty())
            {
                auto& newHousePlace = residentialZones[rand() % residentialZones.size()];
                AddBuilding({newHousePlace.position, sf::Vector2i(1, 1), CityTileType(
                        (rand() % (Index(CityTileType::HOUSE4) - Index(CityTileType::HOUSE1))) +
                        Index(CityTileType::HOUSE1))}, zoneManager, cityMap);
            }
        }
        spawnTimer_.Reset();
    }

}

void
CityBuildingManager::AddBuilding(Building building, const CityZoneManager& zoneManager, CityBuilderMap& cityMap)
{
    cityMap.RemoveCityElement(building.position);
    buildings_.push_back(building);
    std::sort(buildings_.begin(), buildings_.end(), [](const Building& b1, const Building& b2){return (b1.position.y < b2.position.y);});
}


const std::vector<Building>& CityBuildingManager::GetBuildingsVector() const
{
    return buildings_;
}
}