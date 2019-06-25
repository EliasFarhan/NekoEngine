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


#include <engine/engine.h>
#include <city_tilemap.h>
#include <city_map.h>
#include "engine/input.h"

#include <SFML/Window/Event.hpp>
#include "city_command.h"
#include <city_cursor.h>

namespace neko
{
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

	sf::View mainView;
private:
	EntityManager entityManager_;
	TextureManager textureManager_;

	CityCommandManager commandManager_;
	CityBuilderTilemap environmentTilemap_;
	CityBuilderMap cityBuilderMap_;
	CityCursor cursor_;
	const float scrollDelta_ = 0.1f;
	float currentZoom_ = 1.0f;
};
}
