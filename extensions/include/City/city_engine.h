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
#include <core/executor.hpp>
#include "City/city_car.h"
#include "City/city_zone.h"
#include <City/city_building.h>

namespace neko
{

enum class CityComponentType : EntityMask
{
    TRANSFORM = 1 << 0,
    CAR = 1 << 1,
    BUILDING = 1 << 2,
};


class CityBuilderEngine : public neko::MainEngine
{
public:
    void Init() override;

    void Update() override;

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

    sf::View mainView;

    CityBuildingManager& GetBuildingManager();

private:

    tf::Executor executor_{std::thread::hardware_concurrency() - 2};
    EntityManager entityManager_;
    Transform2dManager transformManager_;
    TextureManager textureManager_;

    CityCommandManager commandManager_;
    CityBuilderTilemap environmentTilemap_;
    CityBuilderMap cityBuilderMap_;
    CityCarManager cityCarManager_;
    CityCursor cursor_;
    CityZoneManager cityZoneManager_;
    CityBuildingManager cityBuildingManager_;
    const float scrollDelta_ = 0.1f;
    float currentZoom_ = 1.0f;
};
}
