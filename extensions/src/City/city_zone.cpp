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

#include <City/city_zone.h>
#include <engine/engine.h>
#include <City/city_map.h>
#include <City/city_building.h>
#include <City/city_engine.h>

#ifdef TRACY_ENABLE
#include <Tracy.hpp>
#endif
namespace neko
{


void CityZoneManager::UpdateZoneTilemap(const CityBuilderMap& cityMap, CityBuildingManager& cityBuildingMap,
                                        sf::View mainView)
{

#ifdef TRACY_ENABLE
    ZoneScoped
#endif
    const auto frameIndex = MainEngine::GetInstance()->frameIndex % 2;
    const auto windowView_ = sf::FloatRect((mainView.getCenter() - mainView.getSize() / 2.0f), mainView.getSize());
    zoneVertexArray_[frameIndex].clear();
    zoneVertexArray_[frameIndex].setPrimitiveType(sf::PrimitiveType::Triangles);
    
    const auto tileSize = cityMap.city.tileSize;
    const auto func = [this, &windowView_, &cityBuildingMap, tileSize, frameIndex](const std::vector<Zone>& zones_) {
        for (auto& zone : zones_)
        {
            /*
            //Checking if there is other zone buildings
            const auto* buildingAtPos = cityBuildingMap.GetBuildingAt(zone.position);
            if (buildingAtPos != nullptr)
                continue;
            */
            //culling with mainView
            const auto zoneSize = sf::Vector2f(tileSize);
            const auto worldPos = sf::Vector2f(
                static_cast<float>(zone.position.x * tileSize.x),
                static_cast<float>(zone.position.y * tileSize.y));
            const auto tileRect = sf::FloatRect((worldPos - zoneSize / 2.0f), zoneSize);

            if (!windowView_.intersects(tileRect))
                continue;
            const auto color = zoneColorMap.find(zone.zoneType)->second;
            sf::Vertex quad[6];
            quad[0].position = worldPos - zoneSize / 2.0f;
            quad[1].position = worldPos - sf::Vector2f(zoneSize.x, -zoneSize.y) / 2.0f;
            quad[2].position = worldPos + zoneSize / 2.0f;
            quad[3].position = worldPos - zoneSize / 2.0f;
            quad[4].position = worldPos + zoneSize / 2.0f;
            quad[5].position = worldPos - sf::Vector2f(-zoneSize.x, zoneSize.y) / 2.0f;
            for (auto& v : quad)
            {
                v.color = color;
                zoneVertexArray_[frameIndex].append(v);
            }
        }
    };
    func(residentialZones_);
    func(commercialZones_);
}

void CityZoneManager::PushCommand(GraphicsManager* graphicsManager)
{

#ifdef TRACY_ENABLE
    ZoneScoped
#endif
    const auto frameIndex = MainEngine::GetInstance()->frameIndex % 2;
    graphicsManager->Draw(zoneVertexArray_[frameIndex]);
}

void CityZoneManager::AddZone(sf::Vector2i position, ZoneType zoneType, CityBuilderMap& cityMap)
{

#ifdef TRACY_ENABLE
    ZoneScoped
#endif
	auto* engine = dynamic_cast<CityBuilderEngine*>(MainEngine::GetInstance());
	if(engine->GetCityMoney() < zoneCost)
		return;
	if(position.x < 0 || position.y < 0 || 
        position.x >= static_cast<int>(cityMap.city.mapSize.x) || position.y >= static_cast<int>(cityMap.city.mapSize.y))
	{
		return;
	}
    const auto func = [this, zoneType, engine, position, &cityMap](std::vector<Zone>& zones_) {

        //No zone on water
        if (!cityMap.IsGrass(position))
            return;
        //No zone when there is already a building
        auto* cityElement = cityMap.GetCityElementAt(position);
        if (cityElement != nullptr && cityElement->elementType != CityElementType::TREES)
            return;
        for (auto& road : cityMap.GetRoadGraph().GetNodesVector())
        {
            const auto roadPos = road.position;
            const auto deltaPos = roadPos - position;
            if ((deltaPos.x == 0 && abs(deltaPos.y) <= ZONE_RADIUS) ||
                (deltaPos.y == 0 && abs(deltaPos.x) <= ZONE_RADIUS))
            {
                engine->ChangeCityMoney(-zoneCost);
                zones_.push_back({ position, zoneType });
                break;
            }
        }
    };
    switch (zoneType)
    {
    case ZoneType::COMMERCIAL:
    {
        const auto existingZone = std::find_if(residentialZones_.begin(), residentialZones_.end(), [&position](const Zone& zone) {
            return zone.position == position;
            });
        if (existingZone != residentialZones_.end())
        {
            engine->ChangeCityMoney(-zoneCost);
            existingZone->zoneType = zoneType;
            return;
        }
        func(commercialZones_);
        break;
    }
    case ZoneType::RESIDENTIAL:
    {
        const auto existingZone = std::find_if(commercialZones_.begin(), commercialZones_.end(),
                                               [&position](const Zone& zone)
                                               {
                                                   return zone.position == position;
                                               });
        //TODO the other existing zone
        if (existingZone != commercialZones_.end())
        {
            engine->ChangeCityMoney(-zoneCost);
            existingZone->zoneType = zoneType;
            return;
        }
        func(residentialZones_);
        break;
    }
    default:
        break;
    }
}

void CityZoneManager::RemoveZone(sf::Vector2i position, ZoneType zoneType)
{

#ifdef TRACY_ENABLE
    ZoneScoped
#endif
        const auto func = [this, position](std::vector<Zone>& zones_) {
        const auto existingZone = std::find_if(zones_.begin(), zones_.end(), [&position](const Zone& zone) {
            return zone.position == position;
            });
        if (existingZone != zones_.end())
        {
            zones_.erase(existingZone);
        }
    };
    switch (zoneType)
    {
    case ZoneType::COMMERCIAL:
        func(commercialZones_);
        break;
    case ZoneType::RESIDENTIAL:
        func(residentialZones_);
        break;
    default:
        break;
    }
}

const Zone* CityZoneManager::GetZoneAt(sf::Vector2i position, ZoneType zoneType) const 
{

#ifdef TRACY_ENABLE
    ZoneScoped
#endif
        const auto func = [this, position](const std::vector<Zone>& zones_)-> const Zone*
    {
        const auto existingZone = std::find_if(zones_.begin(), zones_.end(), [&position](const Zone& zone) {
            return zone.position == position;
            });
        if (existingZone == zones_.end())
            return nullptr;
        return &*existingZone;
    };
    switch (zoneType)
    {
    case ZoneType::COMMERCIAL:
        return func(commercialZones_);
    case ZoneType::RESIDENTIAL:
        return func(residentialZones_);
    default:
        break;
    }
    return nullptr;
}

const std::vector<Zone>& CityZoneManager::GetCommericalZoneVector() const
{
    return commercialZones_;
}

const std::vector<Zone>& CityZoneManager::GetResidentialZoneVector() const
{
    return residentialZones_;
}
}