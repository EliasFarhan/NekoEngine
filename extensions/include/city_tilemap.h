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

#include <array>
#include <vector>

#include <graphics/tilemap.h>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/View.hpp>

namespace neko
{

class TextureManager;
class CityBuilderMap;

enum class CityTileType : int
{
	NONE = -1,
	GRASS = 0,
	ROAD_LINE,
	ROAD_TURN,
	CROSS_ROAD,
	ROAD_T,
	PARKING,
	RAIL_TURN,
	RAIL_LINE,
	ROAD_BRIDGE_VERTICAL,
	ROAD_BRIDGE_HORIZONTAL,
	ROAD_ROUNDABOUT,
	CHURCH,
	TREES,
	TRAIN_STATION,
	BUILDING,
	HOUSE,
	WATER_DOWN_RIGHT,
	WATER_DOWN_LEFT,
	WATER_HORIZONTAL,
	WATER_UP_RIGHT,
	WATER_UP_LEFT,
	WATER_VERTICAL,
	LENGTH
};

enum class CityTilesheetType : unsigned
{
    ENVIRONMENT = 0u,
    TRANSPORT,
    CITY,
    CAR,
    LENGTH

};

struct CityBuilderTile
{
	sf::Vector2i position;

};

class CityBuilderTilemap : public neko::Tilemap
{
public:
	void Init(TextureManager& textureManager);

	void UpdateTilemap(CityBuilderMap& cityBuilderMap, sf::View mainView,
                       CityTilesheetType updatedCityTileType = CityTilesheetType::LENGTH);
	void PushCommand(GraphicsManager* graphicsManager) override;
protected:
	void AddNewTile(const sf::Vector2f position, const sf::Vector2f size, const sf::FloatRect rect,
                    const sf::Vector2f center, CityTilesheetType updatedCityTileType, bool flipX,
                    bool flipY, bool rotate90);
	/**
	 * \brief use for tile culling
	 */
	sf::FloatRect windowView_{};
	std::array<Tilesheet, unsigned(CityTilesheetType::LENGTH)> tilesheets_;
    std::array<sf::FloatRect, size_t(CityTileType::LENGTH)> textureRects_;
    std::array<sf::Vector2f, size_t(CityTileType::LENGTH)> rectCenter_;
	const std::string cityTextureName_ = "data/tilemap/CuteCityBuilder.png";
	const std::string carTextureName_ = "data/tilemap/car.png";
	const sf::Vector2i tileSize_ = sf::Vector2i(20, 20);


};
}