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

namespace city
{

const int INIT_PERSON_MONEY = 100;
struct Person
{
	sf::Vector2i position = neko::INVALID_TILE_POS;
	neko::Entity personEntity = neko::INVALID_ENTITY;
	neko::Entity carEntity = neko::INVALID_ENTITY;
	sf::Vector2i housePos = neko::INVALID_TILE_POS;
	sf::Vector2i workPos = neko::INVALID_TILE_POS;
	unsigned dayCount = 0;
	int money = INIT_PERSON_MONEY;
	int salary = 0;
};

class CityPeopleManager : public neko::System
{
public:
	void Init() override;
	void Update(float dt) override;
	void Destroy() override;
	neko::Entity SpawnPerson();
	neko::Entity AddPerson(neko::Entity neko::Entity, sf::Vector2i position);
	Person* GetPersonAt(neko::Entity neko::Entity);
    neko::Index GetPeopleCount();
	void DestroyPerson(neko::Entity neko::Entity);
protected:
	json personBehaviorTree_;
	std::vector<Person> people_;
    neko::Timer spawningTimer_ = neko::Timer(1.0f, .1f);
};
}
