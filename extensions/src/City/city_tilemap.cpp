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
#include <City/city_tilemap.h>
#include <graphics/texture.h>
#include <engine/engine.h>
#include <City/city_map.h>
#include "utilities/json_utility.h"
#include "City/city_car.h"
#include <engine/transform.h>
#include <City/city_building.h>
#include <City/city_engine.h>

#ifdef TRACY_ENABLE
#include <Tracy.hpp>
#endif
namespace neko
{
void CityBuilderTilemap::Init(TextureManager& textureManager)
{

#ifdef TRACY_ENABLE
	ZoneScoped
#endif
	static const auto reverseCityMap =
		[]() ->std::map<std::string, CityTileType>
	{
		std::map<std::string, CityTileType> reverse;
		std::for_each(mapCityTileString.begin(), mapCityTileString.end(),
			[&reverse](const std::pair<CityTileType, std::string>& pair)
		{
			reverse.insert({ pair.second, pair.first });
		});
		return reverse;
	}();
	static const auto reverseCarMap =
		[]() ->std::map<std::string, CarType>
	{
		std::map<std::string, CarType> reverse;
		std::for_each(mapCarTypeString.begin(), mapCarTypeString.end(),
			[&reverse](const std::pair<CarType, std::string>& pair)
		{
			reverse.insert({ pair.second, pair.first });
		});
		return reverse;
	}();

	const std::unique_ptr<json> cityTilemapJsonPtr = LoadJson("data/tilemap/CuteCityBuilder.json");
	const auto cityTilemapJsonContent = *cityTilemapJsonPtr;
	//City texture
	for (int i = 0; i < int(CityTilesheetType::CAR); i++)
	{
		const Index textureIndex = textureManager.LoadTexture(cityTilemapJsonContent["image"]);
		tilesheets_[i].texture = textureManager.GetTexture(textureIndex);
		tilesheets_[i].texture->setRepeated(false);
	}
	//Car texture
	{
		const Index textureIndex = textureManager.LoadTexture(carTextureName_);
		tilesheets_[static_cast<Index>(CityTilesheetType::CAR)].texture = textureManager.GetTexture(textureIndex);
		tilesheets_[static_cast<Index>(CityTilesheetType::CAR)].texture->setRepeated(false);
	}
	for (auto& cityTileElementJson : cityTilemapJsonContent["tile_elements"])
	{
		const auto typeIt = reverseCityMap.find(cityTileElementJson["type"].get<std::string>());
		if (typeIt != reverseCityMap.end())
		{
			const auto typeIndex = static_cast<Index>(typeIt->second);
			rectCenter_[typeIndex] = GetVectorFromJson(cityTileElementJson, "center");
			textureRects_[typeIndex] = GetFloatRectFromJson(cityTileElementJson, "rect");
		}
	}
	const auto cityCarJsonPtr = LoadJson("data/tilemap/car.json");
	const auto cityCarJsonContent = *cityCarJsonPtr;
	for (auto& carElementJson : cityCarJsonContent["elements"])
	{
		const auto typeIt = reverseCarMap.find(carElementJson["type"].get<std::string>());
		if (typeIt != reverseCarMap.end())
		{
			const auto typeIndex = static_cast<Index>(typeIt->second);
			rectCenter_[typeIndex] = GetVectorFromJson(carElementJson, "center");
			textureRects_[typeIndex] = GetFloatRectFromJson(carElementJson, "rect");
		}
	}

	for (int i = 0; i < static_cast<int>(CityTilesheetType::LENGTH); i++)
	{
		tilesheets_[i].tilemap[0].setPrimitiveType(sf::Triangles);
		tilesheets_[i].tilemap[1].setPrimitiveType(sf::Triangles);
	}

}


void CityBuilderTilemap::UpdateTransportTilemap(const CityBuilderMap& cityBuilderMap)
{

	const auto addRoad = [this, &cityBuilderMap](const CityElement& road)
	{

		constexpr auto updatedCityTileType = CityTilesheetType::TRANSPORT;
		bool directions[] = { false, false, false, false };//UP, DOWN, LEFT, RIGHT

		if (road.position.y == 0)
		{
			directions[0] = true;
		}
		if (road.position.y == static_cast<int>(cityBuilderMap.city.mapSize.y - 1))
		{
			directions[1] = true;
		}
		if (road.position.x == 0)
		{
			directions[2] = true;
		}
		if (road.position.x == static_cast<int>(cityBuilderMap.city.mapSize.x - 1))
		{
			directions[3] = true;
		}
		const bool isWater = cityBuilderMap.environmentTiles_[cityBuilderMap.Position2Index(road.position)] ==
			EnvironmentTile::WATER;

		if(cityBuilderMap.ContainsRoad(road.position+sf::Vector2i(0,-1)))
		{

			directions[0] = true;
		}
		if (cityBuilderMap.ContainsRoad(road.position + sf::Vector2i(0, 1)))
		{
			directions[1] = true;
		}
		if (cityBuilderMap.ContainsRoad(road.position + sf::Vector2i(-1, 0)))
		{
			directions[2] = true;
		}
		if (cityBuilderMap.ContainsRoad(road.position + sf::Vector2i(1, 0)))
		{
			directions[3] = true;
		}
		
		const auto position = sf::Vector2f(
			static_cast<float>(road.position.x * tileSize_.x),
			static_cast<float>(road.position.y * tileSize_.y));
		Index roadIndex = 0;

		if ((directions[0] && directions[1] && !directions[2] && !directions[3]) or
			(directions[0] && !directions[1] && !directions[2] && !directions[3]) or
			(!directions[0] && directions[1] && !directions[2] && !directions[3]))
		{
			roadIndex = static_cast<Index>(isWater ? CityTileType::ROAD_BRIDGE_VERTICAL : CityTileType::ROAD_LINE);
			const auto rect = textureRects_[roadIndex];
			const auto center = rectCenter_[roadIndex];
			const auto size = sf::Vector2f(
				static_cast<float>(tileSize_.x),
				(isWater ? 2.0f : 1.0f) * tileSize_.y);

			AddNewCityTile(position, size, rect, center, updatedCityTileType);
		}
		else if ((!directions[0] && !directions[1] && directions[2] && directions[3]) or
			(!directions[0] && !directions[1] && !directions[2] && directions[3]) or
			(!directions[0] && !directions[1] && directions[2] && !directions[3]) or
			(!directions[0] && !directions[1] && !directions[2] && !directions[3]))
		{
			roadIndex = static_cast<Index>(isWater ? CityTileType::ROAD_BRIDGE_HORIZONTAL : CityTileType::ROAD_LINE);
			const auto rect = textureRects_[roadIndex];
			const auto center = rectCenter_[roadIndex];
			const auto size = sf::Vector2f(
				static_cast<float>(tileSize_.x),
				(isWater ? 2.0f : 1.0f) * tileSize_.y);

			AddNewCityTile(position, size, rect, center, updatedCityTileType, false, false, !isWater, true);
		}
		else if (directions[0] && directions[1] && directions[2] && directions[3])
		{
			roadIndex = static_cast<Index>(CityTileType::CROSS_ROAD);
			const auto rect = textureRects_[roadIndex];
			const auto center = rectCenter_[roadIndex];
			const auto size = sf::Vector2f(static_cast<float>(tileSize_.x), static_cast<float>(tileSize_.y));

			AddNewCityTile(position, size, rect, center, updatedCityTileType, false, false, false, true);
		}
		else if (directions[0] && directions[1] && !directions[2] && directions[3])
		{
			roadIndex = static_cast<Index>(CityTileType::ROAD_T);
			const auto rect = textureRects_[roadIndex];
			const auto center = rectCenter_[roadIndex];
			const auto size = sf::Vector2f(static_cast<float>(tileSize_.x), static_cast<float>(tileSize_.y));

			AddNewCityTile(position, size, rect, center, updatedCityTileType, false, false, false, true);
		}
		else if (directions[0] && directions[1] && directions[2] && !directions[3])
		{
			roadIndex = static_cast<Index>(CityTileType::ROAD_T);
			const auto rect = textureRects_[roadIndex];
			const auto center = rectCenter_[roadIndex];
			const auto size = sf::Vector2f(static_cast<float>(tileSize_.x), static_cast<float>(tileSize_.y));

			AddNewCityTile(position, size, rect, center, updatedCityTileType, true, false, false, true);
		}
		else if (directions[0] && !directions[1] && directions[2] && directions[3])
		{
			roadIndex = static_cast<Index>(CityTileType::ROAD_T);
			const auto rect = textureRects_[roadIndex];
			const auto center = rectCenter_[roadIndex];
			const auto size = sf::Vector2f(static_cast<float>(tileSize_.x), static_cast<float>(tileSize_.y));

			AddNewCityTile(position, size, rect, center, updatedCityTileType, false, false, true, true);
		}
		else if (!directions[0] && directions[1] && directions[2] && directions[3])
		{
			roadIndex = static_cast<Index>(CityTileType::ROAD_T);
			const auto rect = textureRects_[roadIndex];
			const auto center = rectCenter_[roadIndex];
			const auto size = sf::Vector2f(static_cast<float>(tileSize_.x), static_cast<float>(tileSize_.y));

			AddNewCityTile(position, size, rect, center, updatedCityTileType, true, false, true, true);
		}
		else if (!directions[0] && directions[1] && !directions[2] && directions[3])
		{
			roadIndex = static_cast<Index>(CityTileType::ROAD_TURN);
			const auto rect = textureRects_[roadIndex];
			const auto center = rectCenter_[roadIndex];
			const auto size = sf::Vector2f(static_cast<float>(tileSize_.x), static_cast<float>(tileSize_.y));

			AddNewCityTile(position, size, rect, center, updatedCityTileType, false, false, false, true);
		}
		else if (!directions[0] && directions[1] && directions[2] && !directions[3])
		{
			roadIndex = static_cast<Index>(CityTileType::ROAD_TURN);
			const auto rect = textureRects_[roadIndex];
			const auto center = rectCenter_[roadIndex];
			const auto size = sf::Vector2f(static_cast<float>(tileSize_.x), static_cast<float>(tileSize_.y));

			AddNewCityTile(position, size, rect, center, updatedCityTileType, true, false, false, true);
		}
		else if (directions[0] && !directions[1] && !directions[2] && directions[3])
		{
			roadIndex = static_cast<Index>(CityTileType::ROAD_TURN);
			const auto rect = textureRects_[roadIndex];
			const auto center = rectCenter_[roadIndex];
			const auto size = sf::Vector2f(static_cast<float>(tileSize_.x), static_cast<float>(tileSize_.y));

			AddNewCityTile(position, size, rect, center, updatedCityTileType, false, true, false, true);
		}
		else if (directions[0] && !directions[1] && directions[2] && !directions[3])
		{
			roadIndex = static_cast<Index>(CityTileType::ROAD_TURN);
			const auto rect = textureRects_[roadIndex];
			const auto center = rectCenter_[roadIndex];
			const auto size = sf::Vector2f(static_cast<float>(tileSize_.x), static_cast<float>(tileSize_.y));

			AddNewCityTile(position, size, rect, center, updatedCityTileType, true, true, false, true);
		}
	};
	const auto addRail = [this, &cityBuilderMap](const CityElement& rail)
	{
		constexpr auto updatedCityTileType = CityTilesheetType::TRANSPORT;
		bool directions[] = { false, false, false, false };//UP, DOWN, LEFT, RIGHT

		if (rail.position.y == 0)
		{
			directions[0] = true;
		}
		if (rail.position.y == static_cast<int>(cityBuilderMap.city.mapSize.y - 1))
		{
			directions[1] = true;
		}
		if (rail.position.x == 0)
		{
			directions[2] = true;
		}
		if (rail.position.x == static_cast<int>(cityBuilderMap.city.mapSize.x - 1))
		{
			directions[3] = true;
		}

		if (cityBuilderMap.ContainsRail(rail.position+sf::Vector2i(0, -1)))
		{
			directions[0] = true;
		}
		if (cityBuilderMap.ContainsRail(rail.position + sf::Vector2i(0, 1)))
		{
			directions[1] = true;
		}
		if (cityBuilderMap.ContainsRail(rail.position + sf::Vector2i(-1, 0)))
		{
			directions[2] = true;
		}
		if (cityBuilderMap.ContainsRail(rail.position + sf::Vector2i(1, 0)))
		{
			directions[3] = true;
		}
	
		const auto position = sf::Vector2f(
			static_cast<float>(rail.position.x * tileSize_.x),
			static_cast<float>(rail.position.y * tileSize_.y));
		//HORIZONTAL FIRST
		if (directions[2] && directions[3])
		{
			constexpr int railIndex = static_cast<int>(CityTileType::RAIL_LINE);
			const auto rect = textureRects_[railIndex];
			const auto center = rectCenter_[railIndex];
			const auto size = sf::Vector2f(tileSize_);

			AddNewCityTile(position, size, rect, center, updatedCityTileType);
		}

		//UP RIGHT
		else if (directions[0] && directions[3])
		{
			constexpr auto railIndex = static_cast<Index>(CityTileType::RAIL_TURN);
			const auto rect = textureRects_[railIndex];
			const auto center = rectCenter_[railIndex];
			const auto size = sf::Vector2f(tileSize_);

			AddNewCityTile(position, size, rect, center, updatedCityTileType);
		}
		//DOWN LEFT
		else if (directions[1] && directions[2])
		{
			constexpr int railIndex = static_cast<int>(CityTileType::RAIL_TURN);
			const auto rect = textureRects_[railIndex];
			const auto center = rectCenter_[railIndex];
			const auto size = sf::Vector2f(tileSize_);

			AddNewCityTile(position, size, rect, center, updatedCityTileType, true, true, false, true);
		}
		//DOWN RIGHT
		else if (directions[1] && directions[3])
		{
			constexpr int railIndex = static_cast<int>(CityTileType::RAIL_TURN);
			const auto rect = textureRects_[railIndex];
			const auto center = rectCenter_[railIndex];
			const auto size = sf::Vector2f(tileSize_);

			AddNewCityTile(position, size, rect, center, updatedCityTileType, false, true, false, true);
		}
		//UP LEFT
		else if (directions[0] && directions[2])
		{
			constexpr int railIndex = static_cast<int>(CityTileType::RAIL_TURN);
			const auto rect = textureRects_[railIndex];
			const auto center = rectCenter_[railIndex];
			const auto size = sf::Vector2f(tileSize_);

			AddNewCityTile(position, size, rect, center, updatedCityTileType, true, false, false, true);
		}
		//VERTICAL LAST
		else if (directions[0] && directions[1])
		{
			constexpr int railIndex = static_cast<int>(CityTileType::RAIL_LINE);
			const auto rect = textureRects_[railIndex];
			const auto center = rectCenter_[railIndex];
			const auto size = sf::Vector2f(tileSize_);

			AddNewCityTile(position, size, rect, center, updatedCityTileType, false, false, true, true);
		}
	};
	//ROAD
	for (const auto& cityElement : cityBuilderMap.elements_)
	{
		if (cityElement.elementType != CityElementType::ROAD && cityElement.elementType != CityElementType::RAIL) continue;

		const sf::FloatRect tileRect = sf::FloatRect(sf::Vector2f(
			static_cast<float>(cityElement.position.x * tileSize_.x - tileSize_.x / 2),
			static_cast<float>(cityElement.position.y * tileSize_.y - tileSize_.y / 2)),
			2.0f * sf::Vector2f(tileSize_));
		if (!windowView_.intersects(tileRect))
			continue;
		if(cityElement.elementType == CityElementType::ROAD)
		{
			addRoad(cityElement);
		}
		else if(cityElement.elementType == CityElementType::RAIL)
		{
			addRail(cityElement);
		}
	}
}

void CityBuilderTilemap::UpdateTilemap(const CityBuilderMap& cityBuilderMap, const CityCarManager& cityCarManager,
	const CityBuildingManager& buildingManager,
	const Transform2dManager& transformManager,
	sf::View mainView, CityTilesheetType updatedCityTileType)
{

#ifdef TRACY_ENABLE
	ZoneScoped;
	ZoneValue(static_cast<std::uint64_t>(updatedCityTileType));
#endif
	const Index frameIndex = MainEngine::GetInstance()->frameIndex % 2;
	//Manage window view
	windowView_ = sf::FloatRect((mainView.getCenter() - mainView.getSize() / 2.0f), mainView.getSize());

	if (updatedCityTileType == CityTilesheetType::LENGTH)
	{
		for (Index i = 0u; i < static_cast<Index>(CityTilesheetType::LENGTH); i++)
		{
			UpdateTilemap(cityBuilderMap, cityCarManager, buildingManager, transformManager, mainView,
				static_cast<CityTilesheetType>(i));
		}
		return;
	}
	
	tilesheets_[static_cast<Index>(updatedCityTileType)].tilemap[frameIndex].clear();

	switch (updatedCityTileType)
	{
		//ENVIRONMENT
	case CityTilesheetType::ENVIRONMENT:
	{
		constexpr auto grassIndex = static_cast<Index>(CityTileType::GRASS);
		//Fill the vertex array with grass
		for (auto x = 0u; x < cityBuilderMap.city.mapSize.x; x++)
		{
			for (auto y = 0u; y < cityBuilderMap.city.mapSize.y; y++)
			{

				const auto position = sf::Vector2f(
					static_cast<float>(x * tileSize_.x),
					static_cast<float>(y * tileSize_.y));
				auto& rect = textureRects_[grassIndex];
				auto& center = rectCenter_[grassIndex];
				AddNewCityTile(position, sf::Vector2f(tileSize_), rect, center, updatedCityTileType);
			}
		}
		//Add water on top of it
		for (auto x = 0u; x < cityBuilderMap.city.mapSize.x; x++)
		{
			for (auto y = 0u; y < cityBuilderMap.city.mapSize.y; y++)
			{

				Index waterIndex = 0u;
				bool directions[] = { false, false, false, false };//UP, DOWN, LEFT, RIGHT

				const sf::Vector2i tmpPos = sf::Vector2i(x, y);
				if (cityBuilderMap.environmentTiles_[cityBuilderMap.Position2Index(tmpPos)] !=
					EnvironmentTile::WATER)
				{
					continue;
				}
				const sf::FloatRect tileRect = sf::FloatRect(sf::Vector2f(
					static_cast<float>(tmpPos.x * tileSize_.x - tileSize_.x / 2),
					static_cast<float>(tmpPos.y * tileSize_.y - tileSize_.y / 2)), sf::Vector2f(tileSize_));

				if (!windowView_.intersects(tileRect))
					continue;
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
					if (static_cast<unsigned int>(rightPos.x) < cityBuilderMap.city.mapSize.x &&
						cityBuilderMap.environmentTiles_[cityBuilderMap.Position2Index(rightPos)] ==
						EnvironmentTile::WATER)
					{
						directions[3] = true;
					}
				}
				if (directions[1] && directions[3])
				{
					waterIndex = static_cast<Index>(CityTileType::WATER_DOWN_RIGHT);
				}
				if (directions[1] && directions[2])
				{
					waterIndex = static_cast<Index>(CityTileType::WATER_DOWN_LEFT);
				}
				if (directions[0] && directions[3])
				{
					waterIndex = static_cast<Index>(CityTileType::WATER_UP_RIGHT);
				}
				if (directions[0] && directions[2])
				{
					waterIndex = static_cast<Index>(CityTileType::WATER_UP_LEFT);
				}
				if (directions[0] && directions[1])
				{
					waterIndex = static_cast<Index>(CityTileType::WATER_VERTICAL);
				}
				if (directions[2] && directions[3])
				{
					waterIndex = static_cast<Index>(CityTileType::WATER_HORIZONTAL);
				}
				const auto position = sf::Vector2f(
					static_cast<float>(x * tileSize_.x),
					static_cast<float>(y * tileSize_.y));
				auto rect = textureRects_[waterIndex];
				auto center = rectCenter_[waterIndex];
				AddNewCityTile(position, sf::Vector2f(tileSize_), rect, center, updatedCityTileType);
			}
		}
		break;
	}
	case CityTilesheetType::TRANSPORT:
	{
		UpdateTransportTilemap(cityBuilderMap);
		break;
	}

	case CityTilesheetType::CITY:
	{
		for (auto& element : cityBuilderMap.elements_)
		{
			switch (element.elementType)
			{
			case CityElementType::TREES:
			{
				//TREES
				const auto treesIndex = static_cast<Index>(CityTileType::TREES);
				if (cityBuilderMap.environmentTiles_[cityBuilderMap.Position2Index(element.position)] ==
					EnvironmentTile::WATER)
				{
					continue;
				}
				const auto position = sf::Vector2f(
					static_cast<float>(element.position.x * tileSize_.x),
					static_cast<float>(element.position.y * tileSize_.y));
				const auto rect = textureRects_[treesIndex];
				const auto center = rectCenter_[treesIndex];
				const auto size = sf::Vector2f(
					static_cast<float>(tileSize_.x),
					2.0f * tileSize_.y);

				AddNewCityTile(position, size, rect, center, updatedCityTileType);
				break;
			}
			case CityElementType::TRAIN_STATION:
			{
				//TRAIN STATION
				const auto trainStationIndex = static_cast<Index>(CityTileType::TRAIN_STATION);
				const auto position = sf::Vector2f(
					static_cast<float>(element.position.x * tileSize_.x),
					static_cast<float>(element.position.y * tileSize_.y));
				const auto rect = textureRects_[trainStationIndex];
				const auto center = rectCenter_[trainStationIndex];
				const auto size = sf::Vector2f(
					static_cast<float>(element.size.x * tileSize_.x),
					static_cast<float>(element.size.y * tileSize_.y));
				AddNewCityTile(position, size, rect, center, updatedCityTileType);
				break;
			}
			default:
				break;
			}
		}

		for (const auto& building : buildingManager.GetBuildingsVector())
		{
			const auto buildingIndex = static_cast<Index>(building.buildingType);
			const auto position = sf::Vector2f(static_cast<float>(building.position.x * tileSize_.x), static_cast<float>(building.position.y * tileSize_.y));
			const auto size = rectCenter_[buildingIndex] * 2.0f;
			AddNewCityTile(position, size, textureRects_[buildingIndex], rectCenter_[buildingIndex], updatedCityTileType);
		}
		break;
	}
	case CityTilesheetType::CAR:
	{
		for (const auto& car : cityCarManager.GetCarsVector())
		{
			if (car.entity == INVALID_ENTITY)
				continue;

			const auto rect = textureRects_[static_cast<Index>(car.carType)];
			const auto center = rectCenter_[static_cast<Index>(car.carType)];
			const auto position = transformManager.GetPosition(car.entity);
			auto deltaPos = car.currentPath[car.currentIndex + 1] - car.currentPath[car.currentIndex];

			AddCar(position, car.spriteSize, rect, center, deltaPos.x > 0 ? false : true, false);
		}
		break;
	}
	default:
		break;
	}
}

void CityBuilderTilemap::PushCommand(GraphicsManager* graphicsManager)
{

#ifdef TRACY_ENABLE
	ZoneScoped
#endif
	const Index frameIndex = MainEngine::GetInstance()->frameIndex % 2;

	{
		auto* engine = dynamic_cast<CityBuilderEngine*>(MainEngine::GetInstance());
        const auto& city = engine->GetCityMap().city;
		auto& rect = bgRect[frameIndex];
		rect.setPosition(sf::Vector2f(0, 0));
		rect.setSize(sf::Vector2f(
			static_cast<float>(tileSize_.x * city.mapSize.x - tileSize_.x / 2),
			static_cast<float>(tileSize_.y * city.mapSize.y - tileSize_.y / 2)));
		rect.setFillColor(sf::Color(191, 206, 30));
		graphicsManager->Draw(rect);
	}

	graphicsManager->Draw(&tilesheets_[static_cast<Index>(CityTilesheetType::ENVIRONMENT)].tilemap[frameIndex],
		tilesheets_[static_cast<unsigned>(CityTilesheetType::ENVIRONMENT)].texture.get());
	graphicsManager->Draw(&tilesheets_[static_cast<Index>(CityTilesheetType::TRANSPORT)].tilemap[frameIndex],
		tilesheets_[static_cast<unsigned>(CityTilesheetType::TRANSPORT)].texture.get());
	graphicsManager->Draw(&tilesheets_[static_cast<Index>(CityTilesheetType::CAR)].tilemap[frameIndex],
		tilesheets_[static_cast<unsigned>(CityTilesheetType::CAR)].texture.get());
	graphicsManager->Draw(&tilesheets_[static_cast<Index>(CityTilesheetType::CITY)].tilemap[frameIndex],
		tilesheets_[static_cast<unsigned>(CityTilesheetType::CITY)].texture.get());


}

void CityBuilderTilemap::AddNewCityTile(const sf::Vector2f position, const sf::Vector2f size, const sf::FloatRect rect,
	const sf::Vector2f center, CityTilesheetType updatedCityTileType, bool flipX,
	bool flipY, bool rotate90, bool culling)
{
	const Index frameIndex = MainEngine::GetInstance()->frameIndex % 2;
	sf::Vector2f sizeOffset = (size - sf::Vector2f(tileSize_)) / 2.0f;
	sizeOffset.y = -sizeOffset.y;
	if (culling)
	{
		const sf::FloatRect tileRect = sf::FloatRect((position - center + sizeOffset), size);

		if (!windowView_.intersects(tileRect))
			return;
	}
	const auto centerPos = position + sizeOffset;
	sf::Vertex quad[6];
	quad[0].position = centerPos - center;
	quad[1].position = centerPos - sf::Vector2f(center.x, -center.y);
	quad[2].position = centerPos + center;
	quad[3].position = centerPos - center;
	quad[4].position = centerPos + center;
	quad[5].position = centerPos - sf::Vector2f(-center.x, center.y);

	if (rotate90)
	{
		quad[0].texCoords = sf::Vector2f(//rect.left + rect.width,rect.top
			flipX ? rect.left : rect.left + rect.width,
			flipY ? rect.top + rect.height : rect.top);
		quad[1].texCoords = sf::Vector2f(//rect.left,rect.top
			flipX ? rect.left + rect.width : rect.left,
			flipY ? rect.top + rect.height : rect.top);
		quad[2].texCoords = sf::Vector2f(//rect.left,rect.top + rect.height
			flipX ? rect.left + rect.width : rect.left,
			flipY ? rect.top : rect.top + rect.height);
		quad[3].texCoords = sf::Vector2f(//rect.left + rect.width,rect.top
			flipX ? rect.left : rect.left + rect.width,
			flipY ? rect.top + rect.height : rect.top);
		quad[4].texCoords = sf::Vector2f(//rect.left,rect.top + rect.height
			flipX ? rect.left + rect.width : rect.left,
			flipY ? rect.top : rect.top + rect.height);
		quad[5].texCoords = sf::Vector2f(//rect.left + rect.width,rect.top + rect.height
			flipX ? rect.left : rect.left + rect.width,
			flipY ? rect.top : rect.top + rect.height);
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
		tilesheets_[static_cast<unsigned>(updatedCityTileType)].tilemap[frameIndex].append(v);
	}
}

void CityBuilderTilemap::AddCar(const sf::Vector2f position, const sf::Vector2f size, const sf::FloatRect rect,
	const sf::Vector2f center, bool flipX, bool culling)
{

	const Index frameIndex = MainEngine::GetInstance()->frameIndex % 2;
	if (culling)
	{
		const sf::FloatRect tileRect = sf::FloatRect((position - center), size);

		if (!windowView_.intersects(tileRect))
			return;
	}
	sf::Vertex quad[6];
	quad[0].position = position - center;
	quad[1].position = position - sf::Vector2f(center.x, -center.y);
	quad[2].position = position + center;
	quad[3].position = position - center;
	quad[4].position = position + center;
	quad[5].position = position - sf::Vector2f(-center.x, center.y);
	quad[0].texCoords = sf::Vector2f(
		flipX ? rect.left + rect.width : rect.left,
		rect.top);
	quad[1].texCoords = sf::Vector2f(
		flipX ? rect.left + rect.width : rect.left,
		rect.top + rect.height);
	quad[2].texCoords = sf::Vector2f(
		flipX ? rect.left : rect.left + rect.width,
		rect.top + rect.height);
	quad[3].texCoords = sf::Vector2f(
		flipX ? rect.left + rect.width : rect.left,
		rect.top);
	quad[4].texCoords = sf::Vector2f(
		flipX ? rect.left : rect.left + rect.width,
		rect.top + rect.height);
	quad[5].texCoords = sf::Vector2f(
		flipX ? rect.left : rect.left + rect.width,
		rect.top);
	for (auto& v : quad)
	{
		tilesheets_[static_cast<Index>(CityTilesheetType::CAR)].tilemap[frameIndex].append(v);
	}
}

}
