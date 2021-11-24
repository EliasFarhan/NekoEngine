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

#ifdef TRACY_ENABLE
#include <Tracy.hpp>
#endif
#include <City/city_engine.h>
#include <City/city_editor.h>

#include <engine/log.h>
#include <sound/sound.h>
#include "City/city_function_map.h"

namespace neko
{
void CityBuilderEngine::Init()
{
#ifdef TRACY_ENABLE
	ZoneScoped
#endif
	config.vSync = true;
	config.windowStyle = sf::Style::Titlebar | sf::Style::Close;
	MainEngine::Init();

	mainView = renderWindow->getView();
	cityBuilderMap_.Init();
	environmentTilemap_.Init(textureManager_);
	graphicsManager_->editor = std::make_unique<CityEditor>();
	graphicsManager_->editor->Init();

	cursor_.Init();
	commandManager_.Init();
	cityCarManager_.Init();

	cityPeopleManager_.Init();
	musicInd_ = Sound::LoadMusic("data/Meydn01.ogg");
	if (!Sound::PlayMusic(musicInd_))
	{
		logDebug("Could not load music...");
	}
	else
	{
		Sound::SetLoop(musicInd_, true);
	}
}

void CityBuilderEngine::Update(float dt)
{
#ifdef TRACY_ENABLE
	ZoneScoped
#endif
	MainEngine::Update(dt);
	tf::Taskflow taskflow;
	auto carsUpdateTask = taskflow.emplace([&]() {cityCarManager_.Update(dt); });

	auto peopleUpdateTask = taskflow.emplace([&]()
	{
		cityPeopleManager_.Update(dt);
	});
	auto btUpdateTask = taskflow.emplace([&]()
	{
		auto btEntities = entityManager_.FilterEntities(EntityMask(CityComponentType::BEHAVIOR_TREE));
		for (auto entity : btEntities)
		{
			behaviorTreeManager_.ExecuteIndex(entity);
		}
	});
	peopleUpdateTask.precede(btUpdateTask);

	auto commandUpdateTask = taskflow.emplace([&]() {commandManager_.Update(dt); });

	auto buildingUpdateTask = taskflow.emplace([&]() { cityBuildingManager_.
		Update(cityZoneManager_, cityBuilderMap_, dt); });
	commandUpdateTask.precede(buildingUpdateTask);
	std::array<tf::Task, int(CityTilesheetType::LENGTH)> tilemapUpdateTasks;
	auto pushCommandTask = taskflow.emplace([&]() {environmentTilemap_.PushCommand(graphicsManager_.get()); });
	auto mainViewUpdateTask = taskflow.emplace([&]() {
		if (mouseManager_.IsButtonPressed(sf::Mouse::Button::Right))
		{
			const auto delta = sf::Vector2f(mouseManager_.GetMouseDelta());
			mainView.setCenter(mainView.getCenter() - currentZoom_ * delta);
		}
		{
			const float keyboardMoveSpeed = 5.0f;
			sf::Vector2f move = sf::Vector2f();
			if (keyboardManager_.IsKeyHeld(sf::Keyboard::Left))
			{
				move.x -= 1;
			}
			if (keyboardManager_.IsKeyHeld(sf::Keyboard::Right))
			{
				move.x += 1;
			}
			if (keyboardManager_.IsKeyHeld(sf::Keyboard::Up))
			{
				move.y -= 1;
			}
			if (keyboardManager_.IsKeyHeld(sf::Keyboard::Down))
			{
				move.y += 1;
			}
			mainView.setCenter(mainView.getCenter() + currentZoom_ * move * keyboardMoveSpeed );
		}
		graphicsManager_->SetView(mainView);
	});
	for (int i = 0; i < int(CityTilesheetType::LENGTH); i++)
	{
		tilemapUpdateTasks[i] = taskflow.emplace(std::bind([&](CityTilesheetType cityTilesheetType) {
			environmentTilemap_.UpdateTilemap(cityBuilderMap_, cityCarManager_, cityBuildingManager_, transformManager_,
				mainView, cityTilesheetType);
		}, CityTilesheetType(i)));

		if (CityTilesheetType(i) == CityTilesheetType::CAR)
		{
			carsUpdateTask.precede(tilemapUpdateTasks[i]);
			btUpdateTask.precede(tilemapUpdateTasks[i]);
		}
		if (CityTilesheetType(i) == CityTilesheetType::CITY)
		{
			buildingUpdateTask.precede(tilemapUpdateTasks[i]);
		}
		commandUpdateTask.precede(tilemapUpdateTasks[i]);
		tilemapUpdateTasks[i].precede(pushCommandTask);
		mainViewUpdateTask.precede(tilemapUpdateTasks[i]);
	}

	auto zoneUpdateTask = taskflow.emplace([&]() {
		cityZoneManager_.UpdateZoneTilemap(cityBuilderMap_,
		                                   cityBuildingManager_, mainView); });
	buildingUpdateTask.precede(zoneUpdateTask);
	commandUpdateTask.precede(zoneUpdateTask);
	auto pushZoneCommandTask = taskflow.emplace([&]() {cityZoneManager_.PushCommand(graphicsManager_.get()); });
	zoneUpdateTask.precede(pushZoneCommandTask);
	pushCommandTask.precede(pushZoneCommandTask);

	auto cursorUpdateTask = taskflow.emplace([&]() {cursor_.Update(dt); });
	pushCommandTask.precede(cursorUpdateTask);


	mainViewUpdateTask.precede(zoneUpdateTask);

	auto editorUpdateTask = taskflow.emplace([&]() {
		graphicsManager_->editor->AddInspectorInfo("FPS", std::to_string(1.0f / dt));
		graphicsManager_->editor->AddInspectorInfo("Budget", std::to_string(cityMoney_)+"$");
		graphicsManager_->editor->AddInspectorInfo("People", std::to_string(cityPeopleManager_.GetPeopleCount()));
	});
	carsUpdateTask.precede(editorUpdateTask);
	btUpdateTask.precede(editorUpdateTask);


	executor_.run(taskflow);
	executor_.wait_for_all();
}

void CityBuilderEngine::OnEvent(sf::Event& event)
{
#ifdef TRACY_ENABLE
	ZoneScoped
#endif
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
#ifdef TRACY_ENABLE
	ZoneScoped
#endif
	Sound::RemoveMusic(musicInd_);
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

CityPeopleManager& CityBuilderEngine::GetPeopleManager()
{
	return cityPeopleManager_;
}

CityBuildingManager& CityBuilderEngine::GetBuildingManager()
{
	return cityBuildingManager_;
}

BehaviorTreeManager& CityBuilderEngine::GetBehaviorTreeManager()
{
	return behaviorTreeManager_;
}

long long CityBuilderEngine::GetCityMoney() const
{
	return cityMoney_;
}

void CityBuilderEngine::ChangeCityMoney(int delta)
{
	cityMoney_ += delta;
}
}
