#include <City/city_person.h>
#include "City/city_engine.h"

namespace neko
{
void CityPeopleManager::Init()
{

	personBehaviorTree_ = R"JSON(
	{
		"type" : "composite_selector",
		"name" : "Person",
		"children" : 
		[
			{
				"type" : "composite_sequence",
				"name" : "House loop",
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
					}
				]
			},
			{
				"type" : "composite_sequence",
				"name" : "Work loop",
				"children" : 
				[
					{
						"type" : "leaf_functional",
						"name" : "Search work",
						"functional" : "FindWork()"
					},
					{
						"type" : "leaf_move_to",
						"name" : "Goto house",
						"to" : "GotoWork(-1,-1)"
					},
					{
						"type" : "leaf_wait",
						"name" : "Sleep in work",
						"delay" : "5.0"
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
}

void CityPeopleManager::Update()
{
	auto* engine = dynamic_cast<CityBuilderEngine*>(MainEngine::GetInstance());

	spawningTimer.Update(engine->dt.asSeconds());
	if(spawningTimer.IsOver())
	{
		auto& entityManager = engine->GetEntityManager();
		auto& btManager = engine->GetBehaviorTreeManager();
		const Entity person = entityManager.CreateEntity();

		auto index = btManager.ParseBehaviorTreeFromJsonIndex(person, personBehaviorTree_);
		assert(index == person);
		entityManager.AddComponentType(person, EntityMask(CityComponentType::BEHAVIOR_TREE));
		spawningTimer.Reset();
	}
}

void CityPeopleManager::Destroy()
{
}
}
