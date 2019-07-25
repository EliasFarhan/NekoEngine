#pragma once
/*
 MIT License

 Copyright (c) 2017 SAE Institute Switzerland AG

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

#include <engine/globals.h>
#include <SFML/System/Vector2.hpp>
#include "engine/system.h"
#include "utilities/time_utility.h"
#include "engine/entity.h"
#include "engine/component.h"

namespace neko
{
class Position2dManager;
}

namespace city
{
class TileMapGraph;
class CityBuilderMap;

enum class CarType : neko::Index;

enum class CarState : neko::Index
{
	ARRIVED,
	MOVING_TO_NEXT_POS,
	RESCHEDULE
};

struct CityCar : neko::Component
{
	CarType carType;
	std::vector<sf::Vector2i> currentPath;
    neko::Index currentIndex = 0;
    neko::Timer movingTimer = neko::Timer(0.0f, 0.1f);//2 tiles per second
	CarState carState = CarState::ARRIVED;
	sf::Vector2i position = neko::INVALID_TILE_POS;
	const sf::Vector2f spriteSize = sf::Vector2f(32.0f, 16.0f);
};

class CityCarManager : public neko::System
{
public:
	void Init() override;
	void Update(float dt) override;
	void Destroy() override;

    neko::Entity SpawnCar(sf::Vector2i position, CarType carType);

    neko::Entity AddCar(neko::Entity entity, CarType carType, sf::Vector2i position = neko::INVALID_TILE_POS);
	const std::vector<CityCar>& GetCarsVector() const;
	void RescheduleCarPathfinding(const sf::Vector2i& removedPosition);
	size_t CountCar() const;
	CityCar* GetCar(neko::Index carEntity);
private:
	std::vector<CityCar> cars_;
    neko::EntityManager* entityManagerPtr_ = nullptr;
    neko::Position2dManager* positionManager_ = nullptr;
	TileMapGraph* roadGraphPtr_ = nullptr;
	CityBuilderMap* cityMap_ = nullptr;
    neko::Timer spawningTimer = neko::Timer(1.0f, 1.0f);
};
}
