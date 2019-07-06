#include <City/city_person.h>

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
						"type" : "leaf_functional",
						"name" : "Goto house",
						"functional" : "GotoHouse()"
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
						"type" : "leaf_functional",
						"name" : "Goto house",
						"functional" : "GotoWork()"
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
						"type" : "leaf_functional",
						"name" : "Goto exit!",
						"functional" : "MoveOut()"
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
}

void CityPeopleManager::Destroy()
{
}
}
