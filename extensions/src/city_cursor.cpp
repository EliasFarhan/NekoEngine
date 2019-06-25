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

#include <city_cursor.h>
#include "engine/globals.h"
#include "engine/engine.h"
#include <city_engine.h>

namespace neko
{
const static sf::Color cursorColor[size_t(ButtonIconType::LENGTH)] =
{
	sf::Color(0,0,255,100),
	sf::Color(255,0,0,100),
};

void CityCursor::Init()
{
	engine_ = dynamic_cast<CityBuilderEngine*>(MainEngine::GetInstance());
}

void CityCursor::Update()
{
	const Index frameIndex = MainEngine::GetInstance()->frameIndex % 2;

	const auto tileSize = sf::Vector2f(engine_->GetCityMap().city.tileSize);
	auto& rect = cursorRect_[frameIndex];
	const auto mouseTilePos = GetMouseTilePos();
	rect.setPosition(sf::Vector2f(mouseTilePos.x*tileSize.x, mouseTilePos.y*tileSize.y)-tileSize/2.0f);

	switch (cursorMode)
	{
	case ButtonIconType::ROAD:
	case ButtonIconType::BULLDOZER:
	{
		rect.setSize(tileSize); 
		rect.setFillColor(cursorColor[Index(cursorMode)]);
		break;
	}
	default:
		rect.setFillColor(sf::Color::Transparent);
		break;
	}

	MainEngine::GetInstance()->GetGraphicsManager()->Draw(rect);
}

void CityCursor::Destroy()
{
}

void CityCursor::OnEvent(sf::Event& event)
{
	if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
	{
		originPos_ = GetMouseTilePos();
	}
	else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
	{

		//Create tile road or destroy it
		switch (cursorMode)
		{
		case ButtonIconType::ROAD:
		{
			auto command = std::make_unique<BuildElementCommand>();
			command->commandType = CityCommandType::CREATE_CITY_ELEMENT;
			command->elementType = CityElementType::ROAD;
			command->position = originPos_;
			engine_->GetCommandManager().AddCommand(std::move(command));
			break;
		}
		case ButtonIconType::BULLDOZER:
		{
			auto command = std::make_unique<DestroyElementCommand>();
			command->commandType = CityCommandType::DELETE_CITY_ELEMENT;
			command->position = originPos_;
			engine_->GetCommandManager().AddCommand(std::move(command));
			break;
		}
		default:
			break;
		}
	}
}

sf::Vector2i CityCursor::GetMouseWorldPos() const
{
	const auto mousePos = engine_->GetMouseManager().GetPosition();
	const auto view = engine_->mainView;

	return  sf::Vector2i(view.getCenter() + engine_->GetCurrentZoom() * (-sf::Vector2f(engine_->config.screenSize)/2.0f + sf::Vector2f(mousePos)));
	
}

sf::Vector2i CityCursor::GetMouseTilePos() const
{

	const auto tileSize = engine_->GetCityMap().city.tileSize;
	const auto worldMousePos = GetMouseWorldPos()+sf::Vector2i(tileSize)/2;
	return sf::Vector2i(
		int(worldMousePos.x) / tileSize.x,
		int(worldMousePos.y) / tileSize.y);
}
}
