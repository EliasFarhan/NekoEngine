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

#include <City/city_building.h>
#include <engine/engine.h>
#include <City/city_engine.h>
#include <City/city_zone.h>


#ifdef TRACY_ENABLE
#include <Tracy.hpp>
#endif

namespace neko
{

void CityBuildingManager::Update(CityZoneManager& zoneManager, CityBuilderMap& cityMap, const float dt)
{

#ifdef TRACY_ENABLE
	ZoneScoped
#endif
    const auto* engine = dynamic_cast<CityBuilderEngine*>(MainEngine::GetInstance());
	spawnTimer_.Update(dt);
	if (spawnTimer_.IsOver() || (engine->GetCheatData() & CheatModeData::QUICK_HOUSE_SPAWN))
	{

		//Add residential building
		{
			const auto& residentialZones = zoneManager.GetResidentialZoneVector();
			
			if (!residentialZones.empty())
			{
                const auto& newHousePlace = residentialZones[rand() % residentialZones.size()];
				AddBuilding({
                                newHousePlace.position,
                                sf::Vector2i(1, 1),
                                static_cast<CityTileType>((rand() % (static_cast<Index>(CityTileType::HOUSE4) - static_cast<Index>(CityTileType::HOUSE1))) +
                                    static_cast<Index>(CityTileType::HOUSE1)),
                                ZoneType::RESIDENTIAL,
                                (rand() % (10u - 5u)) + 5u
                            },
                            zoneManager,
                            cityMap);
			}
		}
		//Add commercial building
		{
			const auto& commercialZones = zoneManager.GetCommericalZoneVector();
			
			if (!commercialZones.empty())
			{
				auto& newWorkPlace = commercialZones[rand() % commercialZones.size()];
				bool buildBig = true;
				for (int dx = 0; dx < 3; dx++)
				{
					for (int dy = 0; dy < 3; dy++)
					{
						const sf::Vector2i newPos = newWorkPlace.position + sf::Vector2i(dx, -dy);
						{
							const auto [building, lock] = GetBuildingAt(newPos);
							if (building != nullptr && building->size != sf::Vector2i(1, 1))
							{
								buildBig = false;
								break;
							}
						}
						const auto* zone = zoneManager.GetZoneAt(newPos, ZoneType::COMMERCIAL);
						if (zone == nullptr || zone->zoneType != newWorkPlace.zoneType)
						{
							buildBig = false;
							break;
						}
					}
					if (!buildBig)
						break;
				}
				if (buildBig)
				{
					for (int dx = 0; dx < 3; dx++)
					{
						for (int dy = 0; dy < 3; dy++)
						{
							const sf::Vector2i newPos = newWorkPlace.position + sf::Vector2i(dx, -dy);
							RemoveBuilding(newPos);
						}
					}
					AddBuilding({
                                    newWorkPlace.position,
                                    sf::Vector2i(3, 3),
                                    static_cast<CityTileType>((rand() % (Index(CityTileType::OFFICE5) - Index(CityTileType::OFFICE2))) + Index(
                                        CityTileType::OFFICE2)),
                                    ZoneType::COMMERCIAL,
                                    ((rand() % (60u - 40u)) + 40u)
                                }, zoneManager, cityMap);
				}
				else
				{
					AddBuilding({
                                    newWorkPlace.position,
                                    sf::Vector2i(1, 1),
                                    CityTileType::OFFICE1,
                                    ZoneType::COMMERCIAL,
                                    (rand() % (20u - 10u)) + 10u
                                }, zoneManager, cityMap);
				}
			}
		}
		spawnTimer_.Reset();
	}

}

void
CityBuildingManager::AddBuilding(Building building, CityZoneManager& zoneManager, CityBuilderMap& cityMap)
{
	std::lock_guard lock(buildingMutex);
	for (int dx = 0; dx < building.size.x; dx++)
	{
		for (int dy = 0; dy < building.size.y; dy++)
		{
			sf::Vector2i newPos = building.position + sf::Vector2i(dx, -dy);
			zoneManager.RemoveZone(newPos, building.zoneType);
			cityMap.RemoveCityElement(newPos);
		}
	}
	buildings_.push_back(building);
	std::ranges::sort(buildings_, [](const Building& b1, const Building& b2) {return (b1.position.y < b2.position.y); });
}


const std::vector<Building>& CityBuildingManager::GetBuildingsVector() const
{
	return buildings_;
}

void CityBuildingManager::RemoveBuilding(sf::Vector2i position)
{
	std::lock_guard lock(buildingMutex);
	const auto buildingIt = std::ranges::find_if(buildings_, [&position](const Building& building)
    {
        for (int dx = 0; dx < building.size.x; dx++)
        {
            for (int dy = 0; dy < building.size.y; dy++)
            {
                if (position == building.position + sf::Vector2i(dx, -dy))
                {
                    return true;
                }
            }
        }
        return false;
    });
	if (buildingIt != buildings_.end())
	{
		buildings_.erase(buildingIt);
	}
}

sf::Vector2i CityBuildingManager::FindBuilding(ZoneType zoneType)
{
	
	switch (zoneType)
	{
	case ZoneType::RESIDENTIAL:
	{
		std::vector<Building*> houseBuildings;
		houseBuildings.reserve(buildings_.size());
		for (auto& building : buildings_)
		{
			if (building.buildingType != CityTileType::HOUSE1 &&
				building.buildingType != CityTileType::HOUSE2 &&
				building.buildingType != CityTileType::HOUSE3 &&
				building.buildingType != CityTileType::HOUSE4)
				continue;
			if (building.occupancy < building.capacity)
			{
				houseBuildings.push_back(&building);
			}
		}
		if (houseBuildings.empty())
			return INVALID_TILE_POS;
		return houseBuildings[rand() % houseBuildings.size()]->position;
	}
	case ZoneType::COMMERCIAL:
	{
        const auto* engine = dynamic_cast<CityBuilderEngine*>(MainEngine::GetInstance());
		std::vector<Building*> workBuildings;
		workBuildings.reserve(buildings_.size());
		for (auto& building : buildings_)
		{
			if (building.buildingType != CityTileType::OFFICE1 &&
				building.buildingType != CityTileType::OFFICE2 &&
				building.buildingType != CityTileType::OFFICE3 &&
				building.buildingType != CityTileType::OFFICE4 &&
				building.buildingType != CityTileType::OFFICE5)
				continue;
			if (building.occupancy < building.capacity - unsigned(float(building.capacity)*engine->workTax))
			{
				workBuildings.push_back(&building);
			}
		}
		if (workBuildings.empty())
			return INVALID_TILE_POS;
		return workBuildings[rand() % workBuildings.size()]->position;
	}
	case ZoneType::INDUSTRY:
		break;
	default:
		break;
	}
	return INVALID_TILE_POS;
}

std::pair<Building*, std::shared_lock<std::shared_mutex>> CityBuildingManager::GetBuildingAt(sf::Vector2i position)
{
	std::shared_lock lock(buildingMutex);
	const auto result = std::ranges::find_if(buildings_, [&position](const Building& building)
    {
        for (int dx = 0; dx < building.size.x; dx++)
        {
            for (int dy = 0; dy < building.size.y; dy++)
            {
                const sf::Vector2i newPos = building.position + sf::Vector2i(dx, -dy);
                if (newPos == position)
                    return true;
            }
        }
        return false;
    });
	if (result == buildings_.end())
	{
		return { nullptr, std::move(lock)};
	}
	return { &*result, std::move(lock) };
}
}