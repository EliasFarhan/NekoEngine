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
#include <City/city_tilemap.h>
#include <City/city_map.h>
#include "engine/input.h"

#include <SFML/Window/Event.hpp>
#include "City/city_command.h"
#include <City/city_cursor.h>
#include <engine/transform.h>
#include "City/city_car.h"
#include "City/city_zone.h"
#include <City/city_building.h>
#include "city_behavior_tree_manager.h"
#include "city_person.h"
#include "engine/worker_system.h"

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

constexpr int roadCost = 100;
constexpr int zoneCost = 1000;
constexpr int personBaseSalary = 100;
constexpr int baseCityBudget = 1'000'000;

struct CheatModeData
{
    using DataType = std::uint16_t;
    enum CheatModeBit : DataType
    {
        NONE = 0u,
        INFINITE_MONEY = 1u << 0u,
        QUICK_PEOPLE_SPAWN = 1u << 1u,
        QUICK_HOUSE_SPAWN = 1u << 2u,
        QUICK_WORK_SPAWN = 1u << 3u,
        QUICK_SPAWN = QUICK_HOUSE_SPAWN | QUICK_PEOPLE_SPAWN | QUICK_WORK_SPAWN,
        CITY_SPAWN_INIT = 1u << 4u
    };
    DataType data = 0;
};

class CityBuilderEngine : public MainEngine
{
public:
    CityBuilderEngine(Configuration* config = nullptr);
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
    PathFindingManager& GetPathFindingManager() { return pathfindingManager_; }

    sf::View mainView;
    float workTax = 0.125;
    float houseTax = 0.125;

    CityBuildingManager& GetBuildingManager();
	BehaviorTreeManager& GetBehaviorTreeManager();

    long long GetCityMoney() const;
	void ChangeCityMoney(int delta);

    [[nodiscard]] CheatModeData::DataType GetCheatData() const { return cheatModeData_.data; }
    void SetCheatData(CheatModeData::DataType dataType) { cheatModeData_.data = dataType; }
private:

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
    PathFindingManager pathfindingManager_;
	BehaviorTreeManager behaviorTreeManager_;

    const float scrollDelta_ = 0.1f;
    float currentZoom_ = 1.0f;
	Index musicInd_ = INDEX_INVALID;

	long long cityMoney_ = baseCityBudget;

    CheatModeData cheatModeData_{};
};
}
