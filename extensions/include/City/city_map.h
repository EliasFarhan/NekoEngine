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
#include <vector>
#include "SFML/System/Vector2.hpp"
#include "engine/system.h"
#include <PerlinNoise.hpp>
#include "engine/globals.h"
#include "City/city_command.h"
#include <City/city_graph.h>
#include <unordered_set>


namespace neko
{

enum class EnvironmentTile : unsigned
{
	GRASS = 0,
	WATER
};

enum class CityElementType : Index
{
	ROAD,
	BRIDGE,
	PARKING,
	RAIL,
	TRAIN_STATION,
	CHURCH,
	TREES,
	BUILDING,
	HOUSE

};

struct BuildElementCommand : CityCommand
{
	CityElementType elementType;
	sf::Vector2i position;
};

struct DestroyElementCommand : CityCommand
{
	sf::Vector2i position;
};



struct City
{
	sf::Vector2u mapSize = sf::Vector2u(512,256);
	float forestRatio = 0.5f;
	float perlinFreq = 20.0f;
	sf::Vector2u tileSize = sf::Vector2u(20, 20);
};

struct CityElement
{
	sf::Vector2i position;
	sf::Vector2u size = sf::Vector2u(1, 1);
	CityElementType elementType;
};



struct RoadElement : CityElement
{
    /**
     * \brief bitwise representation of neighbors using NeighborType direction
     */
    unsigned neighborsBitwise = 0u;
};


// custom specialization of std::hash can be injected in namespace std

struct Vector2iHash
{
	std::size_t operator()(sf::Vector2i const& v) const noexcept
	{
		std::size_t h1 = std::hash<int>{}(v.x);
		std::size_t h2 = std::hash<int>{}(v.y);
		return h1 ^ (h2 << 1); // or use boost::hash_combine
	}
};
class CityBuilderMap : public System
{
public:
	void Init() override;
	void Update(float dt) override;
	void Destroy() override;

	std::size_t Position2Index(sf::Vector2i pos) const;
    [[nodiscard]] sf::Vector2i Index2Position(size_t index) const;
	void AddCityElement(CityElementType cityElement, const sf::Vector2i& position);
	void RemoveCityElement(const sf::Vector2i& position);
	CityElement* GetCityElementAt(sf::Vector2i position);
	bool IsGrass(sf::Vector2i position);
	TileMapGraph& GetRoadGraph();
    [[nodiscard]] std::vector<sf::Vector2i> GetRoadEnds() const;
	City city{};
    [[nodiscard]] bool ContainsRoad(sf::Vector2i pos) const;
	[[nodiscard]] bool ContainsRail(sf::Vector2i pos) const;
private:
    friend class CityBuilderTilemap;
    TileMapGraph roadGraph_;
    std::vector<EnvironmentTile> environmentTiles_;
    std::vector<CityElement> elements_;
	std::unordered_set<sf::Vector2i, Vector2iHash> roadSet;
	std::unordered_set<sf::Vector2i, Vector2iHash> railSet;
};

}
