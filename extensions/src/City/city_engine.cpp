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
CityBuilderEngine::CityBuilderEngine(Configuration* config) :
    MainEngine(config),
    pathfindingManager_(cityBuilderMap_.GetRoadGraph())
{
}

    void CityBuilderEngine::Init()
    {
#ifdef TRACY_ENABLE
        ZoneScoped
#endif
            config.windowStyle = sf::Style::Titlebar | sf::Style::Close;
        MainEngine::Init();

        mainView = renderWindow->getView();
        cityBuilderMap_.Init();
        environmentTilemap_.Init(textureManager_);
        auto* renderWindow = graphicsManager_->editor->renderWindow_;
        graphicsManager_->editor = std::make_unique<CityEditor>();
        graphicsManager_->editor->Init();
        graphicsManager_->editor->renderWindow_ = renderWindow;

        cursor_.Init();
        commandManager_.Init();
        cityCarManager_.Init();
        pathfindingManager_.Init();
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
        if (cheatModeData_.data & CheatModeData::INFINITE_MONEY)
        {
            cityMoney_ = std::numeric_limits<long long>::max();
        }
        
    }

    void CityBuilderEngine::Update(float dt)
    {
#ifdef TRACY_ENABLE
        ZoneScoped
#endif
        MainEngine::Update(dt);
        //Update car task
        const auto carsUpdateTask = std::make_shared<Task>([&]() {cityCarManager_.Update(dt); });
        workerManager_.AddTask(carsUpdateTask, "other");

        //Update people task
        const auto peopleUpdateTask = std::make_shared<Task>([&]()
            {
#ifdef TRACY_ENABLE
                ZoneNamedN(PeopleUpdateTask, "PeopleUpdateTask", true);
#endif
                cityPeopleManager_.Update(dt);
            });
        workerManager_.AddTask(peopleUpdateTask, "other");

        // Generate Behavior tree entities
        std::vector<Entity, StandardAllocator<Entity>> btEntities(StandardAllocator<Entity>{GetFrameAllocator()});
        const auto generateBtEntities = std::make_shared<Task>([&btEntities, this]()
            {
                btEntities = entityManager_.FilterEntities(static_cast<EntityMask>(CityComponentType::BEHAVIOR_TREE));
            });
        generateBtEntities->AddDependency(peopleUpdateTask);
        workerManager_.AddTask(generateBtEntities, "other");

        //Behavior Tree Update tasks
        std::vector<std::shared_ptr<Task>> btUpdateTasks(std::thread::hardware_concurrency()-3);
        for (std::size_t i = 0; i < btUpdateTasks.size(); i++)
        {
            
            btUpdateTasks[i] = std::make_shared<Task>([this, &btEntities, &btUpdateTasks, i]()
                {
#ifdef TRACY_ENABLE
                    ZoneNamedN(BehaviorTreeTask, "BehaviorTreeTask", true);
#endif
                    const auto btEntitiesChunkSize = btEntities.size() / btUpdateTasks.size();
                    const auto beginIndex = i * btEntitiesChunkSize;
                    const auto endIndex = i == btUpdateTasks.size() - 1 ? 
                        btEntities.size() :
                        (i + 1u) * btEntitiesChunkSize;
                    for (std::size_t btEntityIndex = beginIndex; btEntityIndex < endIndex; btEntityIndex++)
                    {
                        behaviorTreeManager_.ExecuteIndex(btEntities[btEntityIndex]);
                    }
                });
            btUpdateTasks[i]->AddDependency(generateBtEntities);
            btUpdateTasks[i]->AddDependency(peopleUpdateTask);
            workerManager_.AddTask(btUpdateTasks[i], "other");
        }

        //Command task
        const auto commandUpdateTask = std::make_shared<Task>([&]() {commandManager_.Update(dt); });
        workerManager_.AddTask(commandUpdateTask, "other");

        //Update building task
        const auto buildingUpdateTask = std::make_shared<Task>([&]() { cityBuildingManager_.
            Update(cityZoneManager_, cityBuilderMap_, dt); });
        buildingUpdateTask->AddDependency(commandUpdateTask);
        workerManager_.AddTask(buildingUpdateTask, "other");

        //Update city tilemap task
        std::array<std::shared_ptr<Task>, static_cast<int>(CityTilesheetType::LENGTH)> tilemapUpdateTasks{};
        const auto pushCommandTask = std::make_shared<Task>([&]() {environmentTilemap_.PushCommand(graphicsManager_.get()); });
        const auto mainViewUpdateTask = std::make_shared<Task>([&]() {
#ifdef TRACY_ENABLE
            ZoneNamedN(MainViewUpdateTask, "MainViewUpdateTask", true);
#endif
            if (mouseManager_.IsButtonPressed(sf::Mouse::Button::Right))
            {
                const auto delta = sf::Vector2f(mouseManager_.GetMouseDelta());
                mainView.setCenter(mainView.getCenter() - currentZoom_ * delta);
            }
            {
                constexpr float keyboardMoveSpeed = 5.0f;
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
                mainView.setCenter(mainView.getCenter() + currentZoom_ * move * keyboardMoveSpeed);
            }
            graphicsManager_->SetView(mainView);
            });
        workerManager_.AddTask(mainViewUpdateTask, "other");

        for (int i = 0; i < static_cast<int>(CityTilesheetType::LENGTH); i++)
        {
            tilemapUpdateTasks[i] = std::make_shared<Task>([&, i]() {
                environmentTilemap_.UpdateTilemap(cityBuilderMap_, cityCarManager_, cityBuildingManager_, transformManager_,
                    mainView, static_cast<CityTilesheetType>(i));
                });

            if (static_cast<CityTilesheetType>(i) == CityTilesheetType::CAR)
            {
                tilemapUpdateTasks[i]->AddDependency(carsUpdateTask);
                std::ranges::for_each(btUpdateTasks, [&tilemapUpdateTasks, i](auto& btUpdateTask) {
                    tilemapUpdateTasks[i]->AddDependency(btUpdateTask);
                    });
            }
            if (static_cast<CityTilesheetType>(i) == CityTilesheetType::CITY)
            {
                tilemapUpdateTasks[i]->AddDependency(buildingUpdateTask);
            }
            tilemapUpdateTasks[i]->AddDependency(commandUpdateTask);
            pushCommandTask->AddDependency(tilemapUpdateTasks[i]);
            tilemapUpdateTasks[i]->AddDependency(mainViewUpdateTask);
            workerManager_.AddTask(tilemapUpdateTasks[i], "other");
        }
        workerManager_.AddTask(pushCommandTask, "other");
        const auto zoneUpdateTask = std::make_shared<Task>([&]()
            {
#ifdef TRACY_ENABLE
                ZoneNamedN(ZoneUpdateTask, "ZoneUpdateTask", true);
#endif
                cityZoneManager_.UpdateZoneTilemap(cityBuilderMap_,
                    cityBuildingManager_, mainView);
            });
        zoneUpdateTask->AddDependency(buildingUpdateTask);
        zoneUpdateTask->AddDependency(commandUpdateTask);
        zoneUpdateTask->AddDependency(mainViewUpdateTask);
        workerManager_.AddTask(zoneUpdateTask, "other");

        const auto pushZoneCommandTask = std::make_shared<Task>([&]() {cityZoneManager_.PushCommand(graphicsManager_.get()); });
        pushZoneCommandTask->AddDependency(zoneUpdateTask);
        pushZoneCommandTask->AddDependency(pushCommandTask);
        workerManager_.AddTask(pushZoneCommandTask, "other");

        const auto cursorUpdateTask = std::make_shared<Task>([&]() {cursor_.Update(dt); });
        cursorUpdateTask->AddDependency(pushCommandTask);
        workerManager_.AddTask(cursorUpdateTask, "other");


        const auto editorUpdateTask = std::make_shared<Task>([&]() {
            graphicsManager_->editor->AddInspectorInfo("FPS", std::to_string(1.0f / dt));
            graphicsManager_->editor->AddInspectorInfo("Budget", std::to_string(cityMoney_) + "$");
            graphicsManager_->editor->AddInspectorInfo("People", std::to_string(cityPeopleManager_.GetPeopleCount()));
            });
        editorUpdateTask->AddDependency(carsUpdateTask);
        std::ranges::for_each(btUpdateTasks, [&editorUpdateTask](const auto& btUpdateTask)
            {
                editorUpdateTask->AddDependency(btUpdateTask);
            });
        
        workerManager_.AddTask(editorUpdateTask, "other");
        
        cursorUpdateTask->Join();
        pushZoneCommandTask->Join();
        editorUpdateTask->Join();
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
        pathfindingManager_.Destroy();
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
        if (cheatModeData_.data & CheatModeData::INFINITE_MONEY)
        {
            return;
        }
        cityMoney_ += delta;
    }
}
