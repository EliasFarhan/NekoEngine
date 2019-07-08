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
#include <City/city_car.h>
#include "engine/engine.h"
#include "City/city_engine.h"
#include <City/city_map.h>
#include <utilities/vector_utility.h>
#include <Remotery.h>

namespace neko
{
void CityCarManager::Init()
{
	auto* engine = dynamic_cast<CityBuilderEngine*>(MainEngine::GetInstance());
	entityManagerPtr_ = &engine->GetEntityManager();
	transformManagerPtr_ = &engine->GetTransformManager();

	cityMap_ = &engine->GetCityMap();
	roadGraphPtr_ = &cityMap_->GetRoadGraph();
}

void CityCarManager::Update(float dt)
{
	rmt_ScopedCPUSample(UpdateCars, 0);
	/*
	{
		const auto position = roadGraphPtr_->GetNodesVector()[rand() % roadGraphPtr_->GetNodesVector().size()].position;
		const auto carType = CarType((rand() % (Index(CarType::LENGTH) - Index(CarType::BUS))) + Index(CarType::BUS));
		SpawnCar(position, carType);
	}

	for (Entity carEntity = 0u; carEntity < cars_.size(); carEntity++)
	{
		if (entityManagerPtr_->HasComponent(carEntity, EntityMask(CityComponentType::CAR) | EntityMask(CityComponentType::TRANSFORM)))
		{
			auto& car = cars_[carEntity];
			switch (car.carState)
			{
			case CarState::ARRIVED:
			{
				const auto newDestination = roadGraphPtr_->GetNodesVector()[rand() % roadGraphPtr_->GetNodesVector().size()].position;
				car.currentPath = roadGraphPtr_->CalculateShortestPath(car.position, newDestination);
				if (!car.currentPath.empty())
				{
					car.currentIndex = 0u;
					car.movingTimer.Reset();
					car.carState = CarState::MOVING_TO_NEXT_POS;
				}
				else
				{
					entityManagerPtr_->DestroyEntity(carEntity);
				}
				break;
			}
			case CarState::MOVING_TO_NEXT_POS:
			{
				car.movingTimer.Update(dt);
				const auto tileSize = cityMap_->city.tileSize;
				sf::Vector2f deltaPos = sf::Vector2f(0.0f, 0.0f);
				if (car.movingTimer.IsOver())
				{
					car.currentIndex++;
					if (car.currentIndex == car.currentPath.size() - 1)
					{
						car.carState = CarState::ARRIVED;
					}
					car.position = car.currentPath[car.currentIndex];
					car.movingTimer.Reset();
				}
				else
				{
					deltaPos = sf::Vector2f(car.currentPath[car.currentIndex + 1u] - car.position) * car.movingTimer.GetCurrentRatio();
					deltaPos = sf::Vector2f(deltaPos.x * tileSize.x, deltaPos.y * tileSize.y);
				}
				const auto carWorldPos = sf::Vector2f(car.position.x * float(tileSize.x), car.position.y * float(tileSize.y));
				transformManagerPtr_->SetPosition(
					carWorldPos + deltaPos,
					carEntity);
				break;
			}
			case CarState::RESCHEDULE:
			{
				if (car.currentPath.empty())
				{
					car.carState = CarState::ARRIVED;
					break;
				}
				const auto newDestination = car.currentPath.back();
				car.currentPath = roadGraphPtr_->CalculateShortestPath(car.position, newDestination);
				if (!car.currentPath.empty())
				{
					car.currentIndex = 0u;
					car.carState = CarState::MOVING_TO_NEXT_POS;
				}
				else
				{
					entityManagerPtr_->DestroyEntity(carEntity);
				}
				break;
			}
			default:
			{
				break;
			}
			}
		}

	}*/

}

void CityCarManager::Destroy()
{
}

Entity CityCarManager::SpawnCar(sf::Vector2i position, CarType carType)
{
	const Entity newCarEntity = entityManagerPtr_->CreateEntity();
	AddCar(newCarEntity, carType, position);
	entityManagerPtr_->AddComponentType(newCarEntity, EntityMask(CityComponentType::CAR));
	auto& car = cars_[newCarEntity];
	car.entity = newCarEntity;
	const auto tileSize = cityMap_->city.tileSize;
	transformManagerPtr_->AddPosition(sf::Vector2f(car.position.x * float(tileSize.x), car.position.y * float(tileSize.y)), newCarEntity);
	entityManagerPtr_->AddComponentType(newCarEntity, EntityMask(CityComponentType::TRANSFORM));
	return newCarEntity;
}

Entity CityCarManager::AddCar(Entity entity, CarType carType, sf::Vector2i position)
{
	ResizeIfNecessary(cars_, entity);
	cars_[entity].carType = carType;
	cars_[entity].currentPath.clear();
	if (position == INVALID_TILE_POS)
	{

	}
	cars_[entity].position = position;
	return entity;
}

const std::vector<CityCar>& CityCarManager::GetCarsVector() const
{
	return cars_;
}

void CityCarManager::RescheduleCarPathfinding(const sf::Vector2i& removedPosition)
{
	for (Entity carEntity = 0u; carEntity < cars_.size(); carEntity++)
	{
		if (entityManagerPtr_->HasComponent(carEntity, EntityMask(CityComponentType::CAR) | EntityMask(CityComponentType::TRANSFORM)))
		{
			auto& car = cars_[carEntity];
			auto posIt = std::find(car.currentPath.begin(), car.currentPath.end(), removedPosition);
			if (posIt != car.currentPath.end())
			{
				car.carState = CarState::RESCHEDULE;
				//Destroy car if it is in the destroyed area
				if (posIt - car.currentPath.begin() == car.currentIndex)
				{
					entityManagerPtr_->DestroyEntity(carEntity);
				}
			}
		}
	}
}

size_t CityCarManager::CountCar() const
{
	Index count = 0;
	for (Entity entity = 0; entity < cars_.size(); entity++)
	{
		if (entityManagerPtr_->HasComponent(entity, EntityMask(CityComponentType::CAR)))
		{
			count++;
		}
	}
	return count;
}

CityCar* CityCarManager::GetCar(Index carEntity)
{
	if (carEntity >= cars_.size())
		return nullptr;
	return &cars_[carEntity];
}
}
