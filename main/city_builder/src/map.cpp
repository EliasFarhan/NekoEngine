#include <city/map.h>
#include <PerlinNoise.hpp>
#include "engine/log.h"
#include <engine/engine.h>
#include <city/engine.h>
#include <sstream>
#include <cstring>
#include <iterator>

namespace neko
{
class MainEngine;

void CityBuilderMap::Init()
{
	//River
	environmentTiles_.resize(size_t(city.mapSize.x) * city.mapSize.y, EnvironmentTile::GRASS);
	{
		sf::Vector2i pos = sf::Vector2i((rand() % (city.mapSize.x / 3)) + city.mapSize.x / 3, 0);
		int previousX = 0;
		int straightY = 0;
		while (pos.y != city.mapSize.y)
		{
			sf::Vector2i dir = sf::Vector2i(0, 0);
			const int randomValue = rand() % 4;

			if (randomValue % 2 == 0 || pos.y == city.mapSize.y - 1)
			{
				dir.y = 1;
				straightY++;
			}
			else
			{
				if (previousX != 0)
				{
					dir.x = previousX;
				}
				else
				{
					dir.x = randomValue % 3 == 1 ? 1 : -1;
				}
				previousX = dir.x;
				straightY = 0;
			}
			if ((pos.x == 0 && dir.x == -1) ||
				(pos.x == city.mapSize.x - 1 && dir.x == 1))
			{
				dir.x = 0;
			}
			if (straightY == 2)
			{
				previousX = 0;
				straightY = 0;
			}


			environmentTiles_[Position2Index(pos)] = EnvironmentTile::WATER;
			pos = pos + dir;
			if (pos.y == city.mapSize.y)
				continue;
		}

	}

	//ROAD & RAILS
	{
		sf::Vector2i pos = sf::Vector2i(0, (rand() % (city.mapSize.y / 3)) + city.mapSize.y / 3);

		for (unsigned int x = 0; x < city.mapSize.x; x++)
		{
			pos.x = x;
			AddCityElement(CityElementType::ROAD, pos);
		}
		//RAIL
		const int trainStationX = (rand() % (city.mapSize.x / 3)) + city.mapSize.x / 3;
		const int railUpX = (rand() % (city.mapSize.x / 6)) + city.mapSize.x / 6;
		const int railDownX = city.mapSize.x - ((rand() % (city.mapSize.x / 6)) + city.mapSize.x / 6);
		pos.x = 0;
		pos.y -= 2;
		for (int x = 0; x < int(city.mapSize.x); x++)
		{
			pos.x = x;

			if (x == trainStationX)
			{
				//Add the train station
				CityElement trainStation{};
				trainStation.position = pos + sf::Vector2i(0, 1);//bottom left
				trainStation.size = sf::Vector2u(5, 3);
				trainStation.elementType = CityElementType::TRAIN_STATION;
				auto* engine = dynamic_cast<CityBuilderEngine*>(MainEngine::GetInstance());
				engine->mainView.setCenter(sf::Vector2f(float(trainStation.position.x)*city.tileSize.x, float(trainStation.position.y)*city.tileSize.y));
				elements_.push_back(trainStation);
			}
			if (x > trainStationX && x < trainStationX + 4)
			{
#ifdef __neko_dbg__
				{
					std::ostringstream oss;
					oss << "No Rail: (" << pos.x << ", " << pos.y << ")";
					logDebug(oss.str());
				}
#endif
				continue;
			}
#ifdef __neko_dbg__
			{
				std::ostringstream oss;
				oss << "Rail: (" << pos.x << ", " << pos.y << ")";
				logDebug(oss.str());
			}
#endif
			//Down track
			CityElement element{};
			if (x < railDownX)
			{
				element.position = pos;
				element.size = sf::Vector2u(1, 1);
				element.elementType = CityElementType::RAIL;
				elements_.push_back(element);
			}
			else if (x == railDownX)
			{
				for (unsigned int y = pos.y; y < city.mapSize.y; y++)
				{
					element.position = sf::Vector2i(railDownX, y);
					element.size = sf::Vector2u(1, 1);
					element.elementType = CityElementType::RAIL;
					elements_.push_back(element);
				}
			}

			//Up track
			if (x > railUpX)
			{
				element.position = pos - sf::Vector2i(0, 1);

				element.elementType = CityElementType::RAIL;
				elements_.push_back(element);
			}
			else if (x == railUpX)
			{
				for (int y = pos.y; y >= 0; y--)
				{
					element.position = sf::Vector2i(railUpX, y);
					element.size = sf::Vector2u(1, 1);
					element.elementType = CityElementType::RAIL;
					elements_.push_back(element);
				}
			}
		}
	}
	//Trees
	{
		siv::PerlinNoise perlinNoise;
		const auto elementsNmb = elements_.size();
		std::vector<CityElement> obstacles(elements_);
		for (auto x = 0u; x < city.mapSize.x; x++)
		{
			for (auto y = 0u; y < city.mapSize.y; y++)
			{
				const sf::Vector2i pos = sf::Vector2i(x, y);

				const auto pnoise = perlinNoise.noise0_1(
					float(pos.x) / city.mapSize.x * city.perlinFreq,
					float(pos.y) / city.mapSize.y * city.perlinFreq);
				auto result = std::find_if(obstacles.begin(), obstacles.end(), [&pos](CityElement& elem)
				{
					for (unsigned int dx = 0; dx < elem.size.x; dx++)
					{
						for (int dy = 0; dy < int(elem.size.y); dy++)
						{
							if (elem.position + sf::Vector2i(dx, -dy) == pos)
								return true;
						}
					}
					return false;
				});

				if (pnoise < city.forestRatio && result == obstacles.end())
				{
					CityElement element{};
					element.position = pos;
					element.size = sf::Vector2u(1, 1);
					element.elementType = CityElementType::TREES;
					elements_.push_back(element);
				}
			}
		}
	}


}

void CityBuilderMap::Update(float dt)
{
}

void CityBuilderMap::Destroy()
{
}

size_t CityBuilderMap::Position2Index(sf::Vector2i pos) const
{
	return size_t(pos.x + size_t(pos.y) * city.mapSize.x);
}

sf::Vector2i CityBuilderMap::Index2Position(size_t index) const
{
	return sf::Vector2i(int(index) % city.mapSize.x, int(index) / city.mapSize.y);
}

void CityBuilderMap::AddCityElement(CityElementType cityElement, const sf::Vector2i& position)
{
	if(position.x < 0 || position.y < 0 || position.x >= int(city.mapSize.x) || position.y >= int(city.mapSize.y))
	{
		return;
	}
	switch (cityElement)
	{
	case CityElementType::ROAD:
	{
		auto elementIt = std::find_if(elements_.begin(), elements_.end(), [&position](const CityElement& element)
		{
			return element.position == position;
		});
		if (elementIt != elements_.end() && elementIt->elementType == CityElementType::TREES)
		{
			elements_.erase(elementIt);
			elementIt = elements_.end();
		}
		if (elementIt == elements_.end() || elementIt->elementType == CityElementType::RAIL)
		{
			CityElement element{};
			element.position = position;
			element.size = sf::Vector2u(1, 1);
			element.elementType = CityElementType::ROAD;
			roadGraph_.AddNode(position);
			elements_.push_back(element);
		}
		break;
	}
	default:
		break;
	}
}

void CityBuilderMap::RemoveCityElement(const sf::Vector2i& position)
{
	//TODO need to check each element positions taking into account the size
	auto elementIt = std::find_if(elements_.begin(), elements_.end(), [&position](const CityElement& element)
	{
		return element.position == position;
	});
	while (elementIt != elements_.end())
	{
		if (elementIt->elementType == CityElementType::ROAD)
		{
			roadGraph_.RemoveNode(position);
		}
		elements_.erase(elementIt);

		elementIt = std::find_if(elements_.begin(), elements_.end(), [&position](const CityElement& element)
		{
			return element.position == position;
		});
	};
}

TileMapGraph& CityBuilderMap::GetRoadGraph()
{
	return roadGraph_;
}

std::vector<sf::Vector2i> CityBuilderMap::GetRoadEnds() const
{
	std::vector<sf::Vector2i> ends;
	for (auto& node : roadGraph_.GetNodesVector())
	{
		if (node.position.x == 0 ||
			node.position.y == 0 ||
			node.position.x == int(city.mapSize.x-1) ||
			node.position.y == int(city.mapSize.y-1))
		{
			ends.push_back(node.position);
		}
	}
	return ends;
}

CityElement* CityBuilderMap::GetCityElementAt(sf::Vector2i position)
{
	const auto result = std::find_if(elements_.begin(), elements_.end(), [&position](const CityElement& cityElement)
	{
		for (int dx = 0; dx < static_cast<int>(cityElement.size.x); dx++)
		{
			for (int dy = 0; dy < static_cast<int>(cityElement.size.y); dy++)
			{
				if (position == cityElement.position + sf::Vector2i(dx, -dy))
					return true;
			}
		}
		return false;
	});
	if (result == elements_.end())
		return nullptr;
	else
		return &*result;
}

bool CityBuilderMap::IsGrass(sf::Vector2i position)
{
	return environmentTiles_[Position2Index(position)] == EnvironmentTile::GRASS;
}
}
