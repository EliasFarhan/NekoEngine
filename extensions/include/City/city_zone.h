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
#include <map>
#include <engine/system.h>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/View.hpp>
#include <engine/globals.h>
#include <City/city_command.h>
#include <City/city_graph.h>

namespace neko
{
class GraphicsManager;
class CityBuilderMap;
class CityBuildingManager;

constexpr Index ZONE_RADIUS = 4;
enum class ZoneType
{
    RESIDENTIAL,
    COMMERCIAL,
    INDUSTRY,
    NONE
};

struct AddZoneCommand : CityCommand
{
    ZoneType zoneType = ZoneType::NONE;
    sf::Vector2i position = INVALID_TILE_POS;
};


const static std::map<ZoneType, sf::Color> zoneColorMap =
{
    {ZoneType::RESIDENTIAL, sf::Color(0,255,0,150)},
    {ZoneType::COMMERCIAL,  sf::Color(0,0,255,150)},
    {ZoneType::INDUSTRY,    sf::Color(0,255,255,150)},
};

struct Zone
{
    sf::Vector2i position = INVALID_TILE_POS;
    ZoneType zoneType = ZoneType::NONE;
};


class CityZoneManager
{
public:
    void
    UpdateZoneTilemap(const CityBuilderMap& cityMap, CityBuildingManager& cityBuildingMap, sf::View mainView);

    void PushCommand(GraphicsManager* graphicsManager);

    void AddZone(sf::Vector2i position, ZoneType zoneType, CityBuilderMap& cityMap);

    void RemoveZone(sf::Vector2i position, ZoneType zoneType);

    [[nodiscard]] const Zone* GetZoneAt(sf::Vector2i position, ZoneType zoneType) const;

    [[nodiscard]] const std::vector<Zone>& GetResidentialZoneVector() const;
    [[nodiscard]] const std::vector<Zone>& GetCommericalZoneVector() const;
private:
    std::vector<Zone> residentialZones_;
    std::vector<Zone> commercialZones_;
    sf::VertexArray zoneVertexArray_[2];

};

}