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
