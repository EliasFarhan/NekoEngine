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

#include <thread>
#include <engine/engine.h>
#include <city/tilemap.h>
#include <city/map.h>
#include "sfml_engine/input.h"

#include <SFML/Window/Event.hpp>
#include "city/command.h"
#include <city/cursor.h>
#include <engine/transform.h>
#include <core/executor.hpp>
#include "city/car.h"
#include "city/zone.h"
#include <city/building.h>
#include "behavior_tree_manager.h"
#include "person.h"

namespace neko
{

enum class CityComponentType : EntityMask
{
	TRANSFORM = 1 << 0,
	CAR = 1 << 1,
	BUILDING = 1 << 2,
	BEHAVIOR_TREE = 1 << 3,
	PERSON = 1 << 4,
};

const int roadCost = 100;
const int zoneCost = 1000;
const int personBaseSalary = 100;
const int baseCityBudget = 1'000'000;

class CityBuilderEngine : public neko::MainEngine
{
public:
    void Init() override;

    void Update(float dt) override;

    void OnEvent(sf::Event& event) override;

    void Destroy() override;

    TextureManager& GetTextureManager();

    CityCommandManager& GetCommandManager();

    CityCursor& GetCursor();

    float GetCurrentZoom() const;

    CityBuilderMap& GetCityMap();

    EntityManager& GetEntityManager();

    Transform2dManager& GetTransformManager();

    CityCarManager& GetCarManager();

    CityZoneManager& GetZoneManager();
    CityPeopleManager& GetPeopleManager();

    sf::View mainView;
    float workTax = 0.125;
    float houseTax = 0.125;

    CityBuildingManager& GetBuildingManager();
	BehaviorTreeManager& GetBehaviorTreeManager();

    long long GetCityMoney() const;
	void ChangeCityMoney(int delta);

private:

    tf::Executor executor_{std::thread::hardware_concurrency() - 2};
    EntityManager entityManager_;
    Transform2dManager transformManager_;
    TextureManager textureManager_;

	CityPeopleManager cityPeopleManager_;
    CityCommandManager commandManager_;
    CityBuilderTilemap environmentTilemap_;
    CityBuilderMap cityBuilderMap_;
    CityCarManager cityCarManager_;
    CityCursor cursor_;
    CityZoneManager cityZoneManager_;
    CityBuildingManager cityBuildingManager_;
	BehaviorTreeManager behaviorTreeManager_;

    const float scrollDelta_ = 0.1f;
    float currentZoom_ = 1.0f;
	Index musicInd_ = INDEX_INVALID;

	long long cityMoney_ = baseCityBudget;
};
}
