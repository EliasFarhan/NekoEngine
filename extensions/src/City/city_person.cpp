#include <City/city_person.h>

namespace neko
{
void CityPeopleManager::Init()
{
	personBehaviorTree_ =
	{
	{"type" , "composite_selector"},
	{"name" , "Person" },
	{"children" , 
	{
		{
			{"type" , "composite_sequence"},
			{"name" , "House loop" },
			{"children", 
			{
				{
					{"type" , "leaf_functional"},
					{"name" , "Search house"},
					{"functional" ,"FindHouse()"}
				},
				{
					{"type" , "composite_sequence"},
					{"name" , "Find House loop" },
					{"children",
					{
						{"type" , "leaf_condition"},
						{"name" , "Find house"},
						{"condition" ,"House(-1,-1)"}
					},
					{
					}
					}
				},
				{
					{"type" , "leaf_move_to"},
					{"name" , "Move to energy source"},
					{"to" , "MoveTo(12, 34)"}
				},
				{
					{"type" , "leaf_wait"},
					{"name" , "Wait for reload"},
					{"delay" , "2.0"}
				}
			}
			}
		},
		{
			{"type" , "composite_sequence"},
			{"name" , "Work loop" },
			{"children",
			{

			}
			}
		}
	}
	}
	};
}

void CityPeopleManager::Update()
{
}

void CityPeopleManager::Destroy()
{
}
}
