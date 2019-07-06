#pragma once
#include "engine/entity.h"
#include "engine/system.h"
#include "utilities/json_utility.h"
#include "utilities/time_utility.h"

namespace neko
{

struct Person
{
	sf::Vector2i position = INVALID_TILE_POS;
	Entity personEntity = INVALID_ENTITY;
	Entity carEntity = INVALID_ENTITY;
	sf::Vector2i housePos = INVALID_TILE_POS;
	sf::Vector2i workPos = INVALID_TILE_POS;

};

class CityPeopleManager : public System
{
public:
	void Init() override;
	void Update(float dt) override;
	void Destroy() override;
	Entity SpawnPerson();
	Entity AddPerson(Entity entity, sf::Vector2i position);
	Person* GetPersonAt(Entity entity);
	Index GetPeopleCount();
	void DestroyPerson(Entity entity);
protected:
	json personBehaviorTree_;
	std::vector<Person> people_;
	Timer spawningTimer_ = Timer(1.0f, .1f);
};
}
