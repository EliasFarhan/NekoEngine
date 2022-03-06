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
#include "city_graph.h"

namespace neko
{
class Transform2dManager;
class TileMapGraph;
class CityBuilderMap;

enum class CarType : std::uint8_t;

enum class CarState : std::uint8_t
{
	ARRIVED,
	MOVING_TO_NEXT_POS,
	RESCHEDULE
};

struct CityCar : Component
{
	
	std::vector<sf::Vector2i> currentPath;
	Index currentIndex = 0;
	PathFindingManager::PathId pathId = 0;
    CarType carType;
	CarState carState = CarState::ARRIVED;
	Timer movingTimer = Timer(0.0f, 0.1f);//2 tiles per second
	sf::Vector2i position = INVALID_TILE_POS;
	const sf::Vector2f spriteSize = sf::Vector2f(32.0f, 16.0f);
};

class CityCarManager : public System
{
public:
	void Init() override;
	void Update(float dt) override;
	void Destroy() override;

	Entity SpawnCar(sf::Vector2i position, CarType carType);

	Entity AddCar(Entity entity, CarType carType, sf::Vector2i position = INVALID_TILE_POS);
	const std::vector<CityCar>& GetCarsVector() const;
	void RescheduleCarPathfinding(const sf::Vector2i& removedPosition);
	size_t CountCar() const;
	std::pair<CityCar*, std::shared_lock<std::shared_mutex>> GetCar(Index carEntity);
private:

	mutable std::shared_mutex carMutex_;
	std::vector<CityCar> cars_;
	EntityManager* entityManagerPtr_ = nullptr;
	Transform2dManager* transformManagerPtr_ = nullptr;
	TileMapGraph* roadGraphPtr_ = nullptr;
	CityBuilderMap* cityMap_ = nullptr;
	Timer spawningTimer = Timer(1.0f, 1.0f);
	PathFindingManager* pathFindingManager_ = nullptr;
};
}
