/*
 MIT License

 Copyright (c) 2019 SAE Institute Switzerland AG

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
#include <sstream>
#include <City/city_engine.h>
#include <City/city_editor.h>

#include <engine/log.h>
#include <Remotery.h>
#include "City/city_function_map.h"

namespace neko
{
void CityBuilderEngine::Init()
{
	config.vSync = true;
	MainEngine::Init();
	cityBuilderMap_.Init();
	environmentTilemap_.Init(textureManager_);
	mainView = renderWindow->getView();
	graphicsManager_->editor = std::make_unique<CityEditor>();
	graphicsManager_->editor->Init();

	cursor_.Init();
	commandManager_.Init();
	cityCarManager_.Init();

	FunctionMap functionMap;
	functionMap.SetFunction("FindHouse", [&](Index entity, const std::vector<double>&) -> bool
	{
		logDebug("Find House "+std::to_string(entity));
		return true;//might or might not find a house
	});
	functionMap.SetFunction("FindWork", [&](Index entity, const std::vector<double>&) -> bool
	{
		logDebug("Find Work " + std::to_string(entity));
		return true;//might or might not find a house
	});
	functionMap.SetFunction("CheckHomeAndWork", [&](Index entity, const std::vector<double>&) -> bool
	{
		logDebug("CheckHomeAndWork " + std::to_string(entity));
		return true;//might or might not find a house
	});
	functionMap.SetFunction("KillMyself", [&](Index entity, const std::vector<double>&) -> bool
	{
		logDebug("Kill Myself " + std::to_string(entity));
		return true;//might or might not find a house
	});
	functionMap.SetFunction("GotoHouse", [&](Index entity, const std::vector<double>&) -> bool
	{

		logDebug("Move To House " + std::to_string(entity));
		return true;//return false means still running
	});
	functionMap.SetFunction("GotoWork", [&](Index entity, const std::vector<double>&) -> bool
	{
		logDebug("Move To Work " + std::to_string(entity));
		return true;//return false means still running
	});
	functionMap.SetFunction("MoveOut", [&](Index entity, const std::vector<double>&) -> bool
	{
		logDebug("Move Out " + std::to_string(entity));
		return true;//return false means still running
	});


	cityPeopleManager_.Init();
	
}

void CityBuilderEngine::Update()
{
	MainEngine::Update();
    rmt_ScopedCPUSample(CityBuilderUpdate, 0);
	const float dt = MainEngine::GetInstance()->dt.asSeconds();
    tf::Taskflow taskflow;
    auto carsUpdateTask = taskflow.emplace([&](){cityCarManager_.Update();});

	auto peopleUpdateTask = taskflow.emplace([&]()
	{
		cityPeopleManager_.Update();
	});
	auto btUpdateTask = taskflow.emplace([&]()
	{
		auto btEntities = entityManager_.FilterEntities(EntityMask(CityComponentType::BEHAVIOR_TREE));
		for(auto entity : btEntities)
		{
			behaviorTreeManager_.ExecuteIndex(entity);
		}
	});
	btUpdateTask.precede(peopleUpdateTask);

	auto commandUpdateTask = taskflow.emplace([&](){commandManager_.Update();});

    auto buildingUpdateTask = taskflow.emplace([&](){ cityBuildingManager_.
		Update(cityZoneManager_, cityBuilderMap_, dt);});
	commandUpdateTask.precede(buildingUpdateTask);
    std::array<tf::Task, int(CityTilesheetType::LENGTH)> tilemapUpdateTasks;
    auto pushCommandTask = taskflow.emplace([&](){environmentTilemap_.PushCommand(graphicsManager_.get());});
    auto mainViewUpdateTask = taskflow.emplace([&](){
        if (mouseManager_.IsButtonPressed(sf::Mouse::Button::Middle))
        {
            const auto delta = sf::Vector2f(mouseManager_.GetMouseDelta());
            mainView.setCenter(mainView.getCenter() - currentZoom_ * delta);
        }
        graphicsManager_->SetView(mainView);
    });
	for(int i = 0; i < int(CityTilesheetType::LENGTH);i++)
    {
        tilemapUpdateTasks[i] = taskflow.emplace(std::bind([&](CityTilesheetType cityTilesheetType){
            environmentTilemap_.UpdateTilemap(cityBuilderMap_, cityCarManager_, cityBuildingManager_, transformManager_,
                                              mainView, cityTilesheetType);
        }, CityTilesheetType(i)));

        if(CityTilesheetType(i) == CityTilesheetType::CAR)
        {
            carsUpdateTask.precede(tilemapUpdateTasks[i]);
        }
        if(CityTilesheetType(i) == CityTilesheetType::CITY)
        {
            buildingUpdateTask.precede(tilemapUpdateTasks[i]);
        }
        commandUpdateTask.precede(tilemapUpdateTasks[i]);
        tilemapUpdateTasks[i].precede(pushCommandTask);
        mainViewUpdateTask.precede(tilemapUpdateTasks[i]);
    }

	auto zoneUpdateTask = taskflow.emplace([&](){
        cityZoneManager_.UpdateZoneTilemap(cityBuilderMap_,
                                           cityBuildingManager_, mainView);});
	buildingUpdateTask.precede(zoneUpdateTask);
	commandUpdateTask.precede(zoneUpdateTask);
	auto pushZoneCommandTask = taskflow.emplace([&](){cityZoneManager_.PushCommand(graphicsManager_.get());});
	zoneUpdateTask.precede(pushZoneCommandTask);
	pushCommandTask.precede(pushZoneCommandTask);

	auto cursorUpdateTask = taskflow.emplace([&](){cursor_.Update();});
	pushCommandTask.precede(cursorUpdateTask);


	mainViewUpdateTask.precede(zoneUpdateTask);

	auto editorUpdateTask = taskflow.emplace([&](){
        graphicsManager_->editor->AddInspectorInfo("FPS", std::to_string(1.0f / dt));
        graphicsManager_->editor->AddInspectorInfo("Cars", std::to_string(cityCarManager_.CountCar()));
	});
	carsUpdateTask.precede(editorUpdateTask);


	executor_.run(taskflow);
	executor_.wait_for_all();
}

void CityBuilderEngine::OnEvent(sf::Event& event)
{
	MainEngine::OnEvent(event);
	cursor_.OnEvent(event);
	if (event.type == sf::Event::MouseWheelScrolled)
	{
		const float wheelDelta = event.mouseWheelScroll.delta;
#ifdef __neko_dbg__
		{
			std::ostringstream oss;
			oss << "Mouse Wheel Delta: " << wheelDelta;
			logDebug(oss.str());
		}
#endif
		const auto size = mainView.getSize();
		currentZoom_ -= wheelDelta * scrollDelta_ * currentZoom_;

		mainView.setSize(size - wheelDelta * scrollDelta_ * size);
	}
}

void CityBuilderEngine::Destroy()
{
	cursor_.Destroy();
	commandManager_.Destroy();
	MainEngine::Destroy();
}

TextureManager& CityBuilderEngine::GetTextureManager()
{
	return textureManager_;
}

CityCommandManager& CityBuilderEngine::GetCommandManager()
{
	return commandManager_;
}

CityCursor& CityBuilderEngine::GetCursor()
{
	return cursor_;
}

float CityBuilderEngine::GetCurrentZoom() const
{
	return currentZoom_;
}

CityBuilderMap& CityBuilderEngine::GetCityMap()
{
	return cityBuilderMap_;
}

EntityManager& CityBuilderEngine::GetEntityManager()
{
	return entityManager_;
}

Transform2dManager& CityBuilderEngine::GetTransformManager()
{
	return transformManager_;
}

CityCarManager& CityBuilderEngine::GetCarManager()
{
	return cityCarManager_;
}

CityZoneManager& CityBuilderEngine::GetZoneManager()
{
    return cityZoneManager_;
}

CityBuildingManager& CityBuilderEngine::GetBuildingManager()
{
    return cityBuildingManager_;
}

BehaviorTreeManager& CityBuilderEngine::GetBehaviorTreeManager()
{
	return behaviorTreeManager_;
}
}
