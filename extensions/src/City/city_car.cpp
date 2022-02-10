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

#ifdef TRACY_ENABLE
#include <Tracy.hpp>
#endif
namespace neko
{
void CityCarManager::Init()
{

#ifdef TRACY_ENABLE
	ZoneScoped
#endif
	auto* engine = dynamic_cast<CityBuilderEngine*>(MainEngine::GetInstance());
	entityManagerPtr_ = &engine->GetEntityManager();
	transformManagerPtr_ = &engine->GetTransformManager();

	cityMap_ = &engine->GetCityMap();
	roadGraphPtr_ = &cityMap_->GetRoadGraph();
}

void CityCarManager::Update(float dt)
{

#ifdef TRACY_ENABLE
	ZoneScoped
#endif

}

void CityCarManager::Destroy()
{
}

Entity CityCarManager::SpawnCar(sf::Vector2i position, CarType carType)
{

#ifdef TRACY_ENABLE
	ZoneScoped
#endif
	const Entity newCarEntity = entityManagerPtr_->CreateEntity();
	AddCar(newCarEntity, carType, position);
	entityManagerPtr_->AddComponentType(newCarEntity, static_cast<EntityMask>(CityComponentType::CAR));
	auto [car, lock] = GetCar(newCarEntity);
	car->entity = newCarEntity;
	const auto tileSize = cityMap_->city.tileSize;
	transformManagerPtr_->AddPosition(sf::Vector2f(car->position.x * static_cast<float>(tileSize.x), car->position.y * static_cast<float>(tileSize.y)), newCarEntity);
	entityManagerPtr_->AddComponentType(newCarEntity, static_cast<EntityMask>(CityComponentType::TRANSFORM));
	return newCarEntity;
}

Entity CityCarManager::AddCar(Entity entity, CarType carType, sf::Vector2i position)
{
#ifdef TRACY_ENABLE
	ZoneScoped
#endif

	{
	    std::lock_guard lock(carMutex_);
	    if (cars_.size() <= entity)
	    {
#ifdef TRACY_ENABLE
			ZoneNamedN(carRealloc, "Car Realloc", true);
#endif
			const auto wantedSize = static_cast<std::size_t>(entity) + 1u;
			const auto newSize = wantedSize + wantedSize / 2;
		    cars_.resize(newSize);
	    }
	}
	std::shared_lock lock(carMutex_);
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

#ifdef TRACY_ENABLE
	ZoneScoped
#endif
	for (Entity carEntity = 0u; carEntity < cars_.size(); carEntity++)
	{
		if (entityManagerPtr_->HasComponent(carEntity, static_cast<EntityMask>(CityComponentType::CAR) | static_cast<EntityMask>(CityComponentType::TRANSFORM)))
		{
			auto [car, lock] = GetCar(carEntity);
			auto posIt = std::ranges::find(car->currentPath, removedPosition);
			if (posIt != car->currentPath.end())
			{
				car->carState = CarState::RESCHEDULE;
				//Destroy car if it is in the destroyed area
				if (posIt - car->currentPath.begin() == car->currentIndex)
				{
					entityManagerPtr_->DestroyEntity(carEntity);
				}
			}
		}
	}
}

size_t CityCarManager::CountCar() const
{

#ifdef TRACY_ENABLE
	ZoneScoped
#endif
	std::shared_lock lock(carMutex_);
	Index count = 0;
	for (Entity entity = 0; entity < cars_.size(); entity++)
	{
		if (entityManagerPtr_->HasComponent(entity, static_cast<EntityMask>(CityComponentType::CAR)))
		{
			count++;
		}
	}
	return count;
}

std::pair<CityCar*, std::shared_lock<std::shared_mutex>>  CityCarManager::GetCar(Index carEntity)
{

#ifdef TRACY_ENABLE
	ZoneScoped
#endif
	std::shared_lock lock(carMutex_);
	if (carEntity >= cars_.size())
		return {nullptr, std::move(lock)};
	return { &cars_[carEntity], std::move(lock) };
}
}
