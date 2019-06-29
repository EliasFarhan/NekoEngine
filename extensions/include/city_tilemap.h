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
#include <map>

#include <graphics/tilemap.h>
#include <SFML/Graphics/Rect.hpp>
#include <SFML/Graphics/View.hpp>

namespace neko
{

class TextureManager;
class CityBuilderMap;

enum class CityTileType : Index
{

	GRASS = 0,
	ROAD_LINE = 1,
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
	LENGTH,
	NONE,
};

// Object type list used to parse the city tile type in json format.
const static std::map<CityTileType, std::string> mapCityTileString =
{
	{CityTileType::GRASS, "grass"},
	{CityTileType::WATER_UP_LEFT, "water_up_left"},
	{CityTileType::WATER_DOWN_LEFT, "water_down_left"},
	{CityTileType::WATER_DOWN_RIGHT, "water_down_right"},
	{CityTileType::WATER_UP_RIGHT, "water_up_right"},
	{CityTileType::WATER_HORIZONTAL, "water_horizontal"},
	{CityTileType::WATER_VERTICAL, "water_vertical"},
	{CityTileType::ROAD_LINE, "road_line"},
	{CityTileType::ROAD_TURN, "road_turn"},
	{CityTileType::CROSS_ROAD, "cross_road"},
	{CityTileType::ROAD_T, "road_t"},
	{CityTileType::ROAD_BRIDGE_HORIZONTAL, "road_bridge_horizontal"},
	{CityTileType::ROAD_BRIDGE_VERTICAL, "road_bridge_vertical"},
	{CityTileType::RAIL_LINE, "rail_line"},
	{CityTileType::RAIL_TURN, "rail_turn"},
	{CityTileType::TREES, "trees"},
	{CityTileType::TRAIN_STATION, "train_station"},

};
enum class CarType : Index
{
	BUS = Index(CityTileType::LENGTH),
	TRUCK,
	BIG_CAR,
	LONG_CAR,
	AVG_CAR,
	SMALL_CAR,
	LENGTH
};

const static std::map<CarType, std::string> mapCarTypeString =
{
	{CarType::BUS, "bus"},
	{CarType::TRUCK, "truck"},
	{CarType::BIG_CAR, "big_car"},
	{CarType::LONG_CAR, "long_car"},
	{CarType::AVG_CAR, "avg_car"},
	{CarType::SMALL_CAR, "small_car"}
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
	void AddNewCityTile(const sf::Vector2f position, const sf::Vector2f size, const sf::FloatRect rect,
		const sf::Vector2f center, CityTilesheetType updatedCityTileType, bool flipX = false,
		bool flipY = false, bool rotate90 = false, bool culling = true);
	void AddCar(const sf::Vector2f position, const sf::Vector2f size, const sf::FloatRect rect,
		const sf::Vector2f center, bool flipX = false, bool culling = true);
	/**
	 * \brief use for tile culling
	 */
	sf::FloatRect windowView_{};
	std::array<Tilesheet, size_t(CityTilesheetType::LENGTH)> tilesheets_;
	std::array<sf::FloatRect, size_t(CarType::LENGTH)> textureRects_;
	std::array<sf::Vector2f, size_t(CarType::LENGTH)> rectCenter_;
	const std::string cityTextureName_ = "data/tilemap/CuteCityBuilder.png";
	const std::string carTextureName_ = "data/tilemap/car.png";
	const sf::Vector2i tileSize_ = sf::Vector2i(20, 20);


};
}