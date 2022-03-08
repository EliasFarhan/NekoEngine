#include <City/city_person.h>
#include "City/city_engine.h"
#include <City/city_building.h>
#include <engine/transform.h>
#include "engine/log.h"

#ifdef TRACY_ENABLE
#include <Tracy.hpp>
#include <TracyC.h>
#endif
namespace neko
{
void CityPeopleManager::Init()
{

#ifdef TRACY_ENABLE
	ZoneScoped
#endif
	people_.resize(INIT_ENTITY_NMB);
	personBehaviorTree_ = R"JSON(
	{
		"type" : "composite_selector",
		"name" : "Person",
		"children" : 
		[
			{
				"type" : "composite_sequence",
				"name" : "Normal loop",
				"children" : 
				[
					{
						"type" : "leaf_functional",
						"name" : "Search house",
						"functional" : "FindHouse()"
					},
					{
						"type" : "leaf_move_to",
						"name" : "Goto house",
						"to" : "GotoHouse(-1,-1)"
					},
					{
						"type" : "leaf_wait",
						"name" : "Sleep in house",
						"delay" : "5.0"
					},
					{
						"type" : "leaf_functional",
						"name" : "Search work",
						"functional" : "FindWork()"
					},
					{
						"type" : "leaf_move_to",
						"name" : "Goto work",
						"to" : "GotoWork(-1,-1)"
					},
					{
						"type" : "leaf_wait",
						"name" : "Sleep in work",
						"delay" : "5.0"
					},
					{
						"type" : "leaf_functional",
						"name" : "Increase day",
						"functional" : "IncreaseDayCount()"
					}
				]
			},
			{
				"type" : "composite_sequence",
				"name" : "Check loop",
				"children" : 
				[
					{
						"type" : "leaf_condition",
						"name" : "Check if found home and work",
						"condition" : "CheckHomeAndWork(0)"
					},
					{
						"type" : "leaf_move_to",
						"name" : "Goto exit!",
						"to" : "MoveOut(-1,-1)"
					},
					{
						"type" : "leaf_functional",
						"name" : "Harakiri",
						"functional" : "KillMyself()"
					}
				]
			}
		]
	})JSON"_json;


	const auto moveToFunc = [this](Index entity, const std::vector<double>&) -> bool
	{
#ifdef TRACY_ENABLE
		ZoneNamedN(moveTo, "Move To Func", true);
#endif
		auto* engine = static_cast<CityBuilderEngine*>(MainEngine::GetInstance());
		Person* personPtr = engine->GetPeopleManager().GetPersonAt(entity);
		if (personPtr == nullptr)
		{
			DestroyPerson(entity);
			return true;
		}
#ifdef TRACY_ENABLE
		TracyCZoneN(carCtx, "Get Car Lock", true);
#endif
		auto [car, lock] = engine->GetCarManager().GetCar(personPtr->carEntity);
#ifdef TRACY_ENABLE
		TracyCZoneEnd(carCtx);
#endif
		
	    if (car == nullptr)
		{
			DestroyPerson(entity);
			return true;
		}
		if(car->carState == CarState::RESCHEDULE)
		{
			auto& pathfindingManager = static_cast<CityBuilderEngine*>(MainEngine::GetInstance())->GetPathFindingManager();
            if(!pathfindingManager.IsPathDone(car->pathId))
            {
				return false;
            }
			car->currentPath = pathfindingManager.GetPath(car->pathId);
			if(car->currentPath.empty())
			{
				engine->GetEntityManager().DestroyEntity(personPtr->carEntity);
				engine->GetEntityManager().DestroyEntity(personPtr->personEntity);
				return true;
			}
			car->carState = CarState::MOVING_TO_NEXT_POS;
		}

	    car->movingTimer.Update(engine->clockDeltatime.asSeconds());
		const auto tileSize = engine->GetCityMap().city.tileSize;
		auto deltaPos = sf::Vector2f(0.0f, 0.0f);
		if (car->movingTimer.IsOver())
		{
			personPtr->position = car->position;
			car->currentIndex++;
			if (car->currentIndex >= car->currentPath.size() - 1)
			{
				car->carState = CarState::ARRIVED;
				car->entity = INVALID_ENTITY;
				engine->GetEntityManager().DestroyEntity(personPtr->carEntity);
				personPtr->carEntity = INVALID_ENTITY;
				return true;
			}
			car->position = car->currentPath[car->currentIndex];
			personPtr->position = car->position;
			car->movingTimer.Reset();
		}
		else
		{
			deltaPos = sf::Vector2f(car->currentPath[1u + car->currentIndex] - car->position) * car->movingTimer.GetCurrentRatio();
			deltaPos = sf::Vector2f(deltaPos.x * tileSize.x, deltaPos.y * tileSize.y);
		}
		const auto carWorldPos = sf::Vector2f(
			car->position.x * static_cast<float>(tileSize.x), 
			car->position.y * static_cast<float>(tileSize.y));
		engine->GetTransformManager().SetPosition(
			carWorldPos + deltaPos,
			personPtr->carEntity);
		return false; //return false means still running
	};

	
	FunctionMap functionMap;
	functionMap.SetFunction("FindHouse", [this](Index entity, const std::vector<double>&) -> bool
	{

		logDebug("Find House " + std::to_string(entity));
		auto* engine = static_cast<CityBuilderEngine*>(MainEngine::GetInstance());
		Person* personPtr = engine->GetPeopleManager().GetPersonAt(entity);
		if (personPtr == nullptr)
		{
			logDebug("[Error] Person entity is supposed to have a Person struct...");
			DestroyPerson(entity);
			return false;
		}
		auto housePos = personPtr->housePos;
		bool searchNewHouse = false;
		if (housePos != INVALID_TILE_POS)
		{
			auto[ building, lock] = engine->GetBuildingManager().GetBuildingAt(housePos);
			if (building == nullptr)
			{
				searchNewHouse = true;
				logDebug("[Warning] There were no building at the Person housePos");
			}
		}
		else
		{
			searchNewHouse = true;
		}
		if (searchNewHouse)
		{
			housePos = engine->GetBuildingManager().FindBuilding(ZoneType::RESIDENTIAL);
			auto[ building, lock] = engine->GetBuildingManager().GetBuildingAt(housePos);
			if (building == nullptr)
			{
				logDebug("[Error] No available building for house");
				return false;
			}
			building->occupancy++;
			personPtr->housePos = housePos;

		}
		if (housePos != INVALID_TILE_POS)
		{
			auto& pathfindingManager = static_cast<CityBuilderEngine*>(MainEngine::GetInstance())->GetPathFindingManager();

			const auto closestCurrentPos = engine->GetCityMap().GetRoadGraph().GetClosestNode(personPtr->position)->position;
			const auto closestHousePos = engine->GetCityMap().GetRoadGraph().GetClosestNode(housePos)->position;
			/*const auto path = engine->GetCityMap().GetRoadGraph().CalculateShortestPath(closestCurrentPos, closestHousePos);
			if (path.empty())
			{
				logDebug("[Error] Empty path to house pos from Person pos");
				DestroyPerson(entity);
				return false;
			}*/
			personPtr->carEntity = engine->GetCarManager().SpawnCar(closestCurrentPos, CarType::AVG_CAR);
			auto [car, lock] = engine->GetCarManager().GetCar(personPtr->carEntity);
		    //car->currentPath = path;
			car->pathId = pathfindingManager.SchedulePathFinding(closestCurrentPos, closestHousePos);
			car->carState = CarState::RESCHEDULE;
			car->currentIndex = 0u;
			car->movingTimer.Reset();

		}
		return housePos != INVALID_TILE_POS;
	});
	functionMap.SetFunction("FindWork", [this](Index entity, const std::vector<double>&) -> bool
	{
		logDebug("Find Work " + std::to_string(entity));
		auto* engine = static_cast<CityBuilderEngine*>(MainEngine::GetInstance());
		Person* personPtr = engine->GetPeopleManager().GetPersonAt(entity);
		if (personPtr == nullptr)
		{
			logDebug("[Error] Person entity is supposed to have a Person struct...");
			DestroyPerson(entity);
			return false;
		}
		auto workPos = personPtr->workPos;
		bool searchNewWork = false;
		if (workPos != INVALID_TILE_POS)
		{
			auto[ building, lock] = engine->GetBuildingManager().GetBuildingAt(workPos);
			if (building == nullptr)
			{
				searchNewWork = true;
				logDebug("[Warning] There were no building at the Person workPos");
			}
		}
		else
		{
			searchNewWork = true;
		}
		if (searchNewWork)
		{
			workPos = engine->GetBuildingManager().FindBuilding(ZoneType::COMMERCIAL);
			auto[ building, lock] = engine->GetBuildingManager().GetBuildingAt(workPos);
			if (building == nullptr)
			{
				logDebug("[Error] No available building for work");
				return false;
			}
			building->occupancy++;
			personPtr->workPos = workPos;

		}
		if (workPos != INVALID_TILE_POS)
		{
			auto& pathfindingManager = static_cast<CityBuilderEngine*>(MainEngine::GetInstance())->GetPathFindingManager();

			const auto closestCurrentPos = engine->GetCityMap().GetRoadGraph().GetClosestNode(personPtr->position)->position;
			const auto closestWorkPos = engine->GetCityMap().GetRoadGraph().GetClosestNode(workPos)->position;
			/*const auto path = engine->GetCityMap().GetRoadGraph().CalculateShortestPath(closestCurrentPos, closestWorkPos);
			if (path.empty())
			{
				logDebug("[Error] Empty path to work pos from Person pos");
				DestroyPerson(entity);
				return false;
			}*/
			personPtr->carEntity = engine->GetCarManager().SpawnCar(closestCurrentPos, CarType::BIG_CAR);
			auto [car, lock] = engine->GetCarManager().GetCar(personPtr->carEntity);
			//car->currentPath = path;
			car->pathId = pathfindingManager.SchedulePathFinding(closestCurrentPos, closestWorkPos);
			car->carState = CarState::RESCHEDULE;
			car->currentIndex = 0u;
			car->movingTimer.Reset();

		}
		return workPos != INVALID_TILE_POS;
	});
	functionMap.SetFunction("CheckHomeAndWork", [this](Index entity, const std::vector<double>&) -> bool
	{
		auto* engine = static_cast<CityBuilderEngine*>(MainEngine::GetInstance());
		logDebug("CheckHomeAndWork " + std::to_string(entity));
		auto* person = GetPersonAt(entity);
		if (person == nullptr)
			return true;

		auto housePos = person->housePos;
		bool searchNewHouse = false;
		{
			auto [building, lock] = engine->GetBuildingManager().GetBuildingAt(housePos);
			if (building == nullptr)
				searchNewHouse = true;
		}
		if (searchNewHouse)
		{
			housePos = engine->GetBuildingManager().FindBuilding(ZoneType::RESIDENTIAL);
			person->housePos = housePos;
		}

		auto workPos = person->workPos;
		bool searchNewWork = false;
		{
			auto [building, lock] = engine->GetBuildingManager().GetBuildingAt(housePos);
			if (building == nullptr)
				searchNewWork = true;
		}
		if (searchNewWork)
		{
			workPos = engine->GetBuildingManager().FindBuilding(ZoneType::COMMERCIAL);
			person->workPos = workPos;
		}

		const bool leaving = housePos == INVALID_TILE_POS || workPos == INVALID_TILE_POS;
		if(leaving)
		{
			//Pathfinding to exit
			auto& pathfindingManager = static_cast<CityBuilderEngine*>(MainEngine::GetInstance())->GetPathFindingManager();

			const auto ends = engine->GetCityMap().GetRoadEnds();
			const auto finalPos = ends[rand() % ends.size()];
			const auto closestCurrentPos = engine->GetCityMap().GetRoadGraph().GetClosestNode(person->position)->position;
			const auto closestFinalPos = engine->GetCityMap().GetRoadGraph().GetClosestNode(finalPos)->position;
			/*const auto path = engine->GetCityMap().GetRoadGraph().CalculateShortestPath(closestCurrentPos, closestFinalPos);
			if (path.empty())
			{
				logDebug("[Error] Empty path to house pos from Person pos");
				DestroyPerson(entity);
				return false;
			}*/
			person->carEntity = engine->GetCarManager().SpawnCar(closestCurrentPos, CarType::BUS);
			auto [car, lock] = engine->GetCarManager().GetCar(person->carEntity);
			//car->currentPath = path;
			car->pathId = pathfindingManager.SchedulePathFinding(closestCurrentPos, closestFinalPos);
			car->carState = CarState::RESCHEDULE;
			car->currentIndex = 0u;
			car->movingTimer.Reset();
		}
		return leaving;
	});
	functionMap.SetFunction("KillMyself", [&](Index entity, const std::vector<double>&) -> bool
	{
		logDebug("Kill Myself " + std::to_string(entity));
		DestroyPerson(entity);
		return true;//might or might not find a house
	});
	functionMap.SetFunction("IncreaseDayCount", [&](Index entity, const std::vector<double>&)->bool
	{
		auto* engine = static_cast<CityBuilderEngine*>(MainEngine::GetInstance());
		auto* person = GetPersonAt(entity);
		if( person != nullptr)
		{
			person->dayCount++;
			person->salary = static_cast<int>(static_cast<float>(personBaseSalary) * std::pow(1.1f, static_cast<float>(person->dayCount)));
			const int houseTaxValue = static_cast<int>(static_cast<float>(person->salary) * engine->houseTax);
			person->money += person->salary - houseTaxValue;
			engine->ChangeCityMoney(houseTaxValue);
		}
		return true;
	});
	functionMap.SetFunction("GotoHouse", moveToFunc);
	functionMap.SetFunction("GotoWork", moveToFunc);
	functionMap.SetFunction("MoveOut", moveToFunc);
}

void CityPeopleManager::Update(float dt)
{

#ifdef TRACY_ENABLE
	ZoneScoped;
#endif
    const auto* engine = static_cast<CityBuilderEngine*>(MainEngine::GetInstance());
	const auto cheatData = engine->GetCheatData();
	if(cheatData & CheatModeData::QUICK_PEOPLE_SPAWN)
	{
		SpawnPerson();
	}
	else
	{
		spawningTimer_.Update(dt);
		if (spawningTimer_.IsOver())
		{
			SpawnPerson();
			spawningTimer_.Reset();
		}
	}
}

void CityPeopleManager::Destroy()
{
}

Entity CityPeopleManager::SpawnPerson()
{

#ifdef TRACY_ENABLE
	ZoneScoped
#endif
	auto* engine = static_cast<CityBuilderEngine*>(MainEngine::GetInstance());
	const auto mapEnds = engine->GetCityMap().GetRoadEnds();
	if (mapEnds.empty())
		return INVALID_ENTITY;
	auto& entityManager = engine->GetEntityManager();
	auto& btManager = engine->GetBehaviorTreeManager();
	const Entity person = entityManager.CreateEntity();

	const auto index = btManager.ParseBehaviorTreeFromJsonIndex(person, personBehaviorTree_);
	assert(index == person);
	entityManager.AddComponentType(person, static_cast<EntityMask>(CityComponentType::PERSON));
	entityManager.AddComponentType(person, static_cast<EntityMask>(CityComponentType::BEHAVIOR_TREE));
	AddPerson(person, mapEnds[rand() % mapEnds.size()]);
	return person;
}

Entity CityPeopleManager::AddPerson(Entity entity, sf::Vector2i position)
{

#ifdef TRACY_ENABLE
	ZoneScoped
#endif
	const Person person = { position, entity };
	if (entity == INVALID_ENTITY)
	{
		people_.push_back(person);
		return static_cast<Index>(people_.size());
	}
	if (people_.size() <= entity)
	{
		size_t futureSize = people_.size();
		while (futureSize <= entity)
		{
			futureSize *= 2;
		}
		people_.resize(futureSize);
	}

	people_[entity] = person;
	return entity;
}

Person* CityPeopleManager::GetPersonAt(Entity entity)
{

#ifdef TRACY_ENABLE
	ZoneScoped
#endif
	if (entity >= people_.size())
	{
		return nullptr;
	}
	return &people_[entity];
}

Index CityPeopleManager::GetPeopleCount()
{

#ifdef TRACY_ENABLE
	ZoneScoped
#endif
	auto* engine = static_cast<CityBuilderEngine*>(MainEngine::GetInstance());
	return static_cast<Index>(std::ranges::count_if(people_, [&engine](const Person& person)
    {
        return engine->GetEntityManager().HasComponent(person.personEntity, static_cast<EntityMask>(CityComponentType::PERSON)) &&
            person.housePos != INVALID_TILE_POS;
    }));
}

void CityPeopleManager::DestroyPerson(Entity entity)
{

#ifdef TRACY_ENABLE
	ZoneScoped
#endif
		if (entity == INVALID_ENTITY)
			return;
	auto* engine = static_cast<CityBuilderEngine*>(MainEngine::GetInstance());
	auto* person = GetPersonAt(entity);
	if (person == nullptr)
		return;
	{
	    auto [house, lock] = engine->GetBuildingManager().GetBuildingAt(person->housePos);
	    if (house != nullptr)
	    {
		    house->occupancy--;
	    }
    }
	{
		auto[ work, lock] = engine->GetBuildingManager().GetBuildingAt(person->housePos);
		if (work != nullptr)
		{
			work->occupancy--;
		}
	}
	person->carEntity = INVALID_ENTITY;
	engine->GetEntityManager().DestroyEntity(entity);
}
}
