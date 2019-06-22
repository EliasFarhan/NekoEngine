#include <city_map.h>
#include <PerlinNoise.hpp>
#include "engine/log.h"
#include <sstream>
#include <cstring>
#include <iterator>

namespace neko
{
void CityBuilderMap::Init()
{
    //River
    environmentTiles_.resize(city.mapSize.x * city.mapSize.y, EnvironmentTile::GRASS);
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
            CityElement element{};
            element.position = pos;
            element.size = sf::Vector2u(1, 1);
            element.elementType = CityElementType::ROAD;
            elements_.push_back(element);
        }
        //RAIL
        const int trainStationX = (rand() % (city.mapSize.x / 3)) + city.mapSize.x / 3;
        const int railUpX = (rand() % (city.mapSize.x / 6)) + city.mapSize.x / 6;
        const int railDownX = city.mapSize.x - ((rand() % (city.mapSize.x / 6)) + city.mapSize.x / 6);
        pos.x = 0;
        pos.y -= 2;
        for (int x = 0; x < static_cast<int>(city.mapSize.x); x++)
        {
            pos.x = x;

            if (x == trainStationX)
            {
                //Add the train station
                CityElement trainStation{};
                trainStation.position = pos + sf::Vector2i(0, 1);
                trainStation.size = sf::Vector2u(5, 3);
                trainStation.elementType = CityElementType::TRAIN_STATION;
                elements_.push_back(trainStation);
            }
            if (x > trainStationX && x < trainStationX + 4)
            {
                {
                    std::ostringstream oss;
                    oss << "No Rail: (" << pos.x << ", " << pos.y << ")";
                    logDebug(oss.str());
                }
                continue;
            }
            {
                std::ostringstream oss;
                oss << "Rail: (" << pos.x << ", " << pos.y << ")";
                logDebug(oss.str());
            }
            //Down track
            CityElement element{};
            if(x < railDownX)
            {
                element.position = pos;
                element.size = sf::Vector2u(1, 1);
                element.elementType = CityElementType::RAIL;
                elements_.push_back(element);
            }
            else if(x == railDownX)
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
            if(x > railUpX)
            {
                element.position = pos-sf::Vector2i(0,1);

                element.elementType = CityElementType::RAIL;
                elements_.push_back(element);
            }
            else if(x == railUpX)
            {
                for(int y = pos.y; y >= 0;y--)
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
                        for (int dy = 0; dy < static_cast<int>(elem.size.y); dy++)
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

void CityBuilderMap::Update()
{
}

void CityBuilderMap::Destroy()
{
}

size_t CityBuilderMap::Position2Index(sf::Vector2i pos) const
{
    return pos.x + pos.y * city.mapSize.x;
}

sf::Vector2i CityBuilderMap::Index2Position(size_t index) const
{
    return sf::Vector2i(index % city.mapSize.x, index / city.mapSize.y);
}
}
