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
#include "engine/entity.h"
#include "engine/system.h"
#include "utilities/json_utility.h"
#include "utilities/time_utility.h"

namespace neko
{

const int INIT_PERSON_MONEY = 100;
struct Person
{
	sf::Vector2i position = INVALID_TILE_POS;
	Entity personEntity = INVALID_ENTITY;
	Entity carEntity = INVALID_ENTITY;
	sf::Vector2i housePos = INVALID_TILE_POS;
	sf::Vector2i workPos = INVALID_TILE_POS;
	unsigned dayCount = 0;
	int money = INIT_PERSON_MONEY;
	int salary = 0;
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
