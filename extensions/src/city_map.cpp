#include <city_map.h>
#include <PerlinNoise.hpp>
#include "engine/log.h"
#include <sstream>
namespace neko
{
void CityBuilderMap::Init()
{
	//River
	environmentTiles.resize(city.mapSize.x*city.mapSize.y, EnvironmentTile::GRASS);
	{
		sf::Vector2i pos = sf::Vector2i(rand()%city.mapSize.x, 0);
		int previousX = 0;
		int straightY = 0;
		while(pos.y != city.mapSize.y)
		{
			sf::Vector2i dir = sf::Vector2i(0, 0);
			const int randomValue = rand() % 4;

			if(randomValue % 2 == 0 || pos.y == city.mapSize.y-1)
			{
				dir.y = 1;
				straightY++;
			}
			else
			{
				if(previousX != 0)
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
			if((pos.x == 0 && dir.x == -1)||
				(pos.x == city.mapSize.x-1 && dir.x == 1))
			{
				dir.x = 0;
			}
			if(straightY == 2)
			{
				previousX = 0;
				straightY = 0;
			}
			{
				std::ostringstream oss;
				oss << "Direction: (" << dir.x << ", " << dir.y <<")";
				logDebug(oss.str());
			}
			pos = pos + dir;
			if(pos.y == city.mapSize.y)
				continue;
			environmentTiles[Position2Index(pos)] = EnvironmentTile::WATER;
		}

	}
	//ROAD
	{
		sf::Vector2i pos = sf::Vector2i(0, (rand() % (city.mapSize.y/3))+ city.mapSize.y / 3);

		for (int x = 0; x < city.mapSize.x; x++)
		{
			pos.x = x;
			CityElement element{};
			element.position = pos;
			element.size = sf::Vector2u(1, 1);
			element.elementType = CityElementType::ROAD;
			elements.push_back(element);
		}
	}
	//Trees
	{
		siv::PerlinNoise perlinNoise;
		std::vector<CityElement> roads;
		std::copy_if(elements.begin(), elements.end(), std::back_inserter(roads), [](CityElement& elem)
		{
			return elem.elementType == CityElementType::ROAD;
		});
		for (auto x = 0u; x < city.mapSize.x; x++)
		{
			for (auto y = 0u; y < city.mapSize.y; y++)
			{
				const sf::Vector2i pos = sf::Vector2i(x, y);

				const auto pnoise = perlinNoise.noise0_1(
					float(pos.x) / city.mapSize.x*city.perlinFreq,
					float(pos.y) / city.mapSize.y*city.perlinFreq);

				auto result = std::find_if(roads.begin(), roads.end(), [&pos](CityElement& elem)
				{
					return elem.position == pos;
				});

				if (pnoise < city.forestRatio && result == roads.end())
				{
					CityElement element{};
					element.position = pos;
					element.size = sf::Vector2u(1, 1);
					element.elementType = CityElementType::TREES;
					elements.push_back(element);
				}
			}
		}
	}
	

}
void CityBuilderMap::Update()
{
}
void CityBuilderMap::Destroy()
{
}

size_t CityBuilderMap::Position2Index(sf::Vector2i pos) const
{
	return pos.x * city.mapSize.x + pos.y;
}

sf::Vector2i CityBuilderMap::Index2Position(size_t index) const
{
	return sf::Vector2i(index%city.mapSize.x, index / city.mapSize.y);
}
}
