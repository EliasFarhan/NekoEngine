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


namespace neko
{

enum class EnvironmentTile : unsigned
{
	GRASS = 0,
	WATER
};

enum class CityElementType : unsigned 
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

struct City
{
	sf::Vector2u mapSize = sf::Vector2u(100,100);
	float forestRatio = 0.5f;
	float perlinFreq = 20.0f;
};

struct CityElement
{
	sf::Vector2i position;
	sf::Vector2u size = sf::Vector2u(1, 1);
	CityElementType elementType;
};

class CityBuilderMap : public System
{
public:
	void Init() override;
	void Update() override;
	void Destroy() override;

	size_t Position2Index(sf::Vector2i pos) const;
	sf::Vector2i Index2Position(size_t index) const;

	City city;
	std::vector<EnvironmentTile> environmentTiles;
	std::vector<CityElement> elements;
};

}
