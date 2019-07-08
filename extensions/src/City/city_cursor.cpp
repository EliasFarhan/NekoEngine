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
#include <cmath>
#include <City/city_cursor.h>
#include "engine/globals.h"
#include "engine/engine.h"
#include <City/city_engine.h>

namespace neko
{
const static sf::Color cursorColor[size_t(ButtonIconType::LENGTH)] =
{
		sf::Color(0, 0, 255, 100),
		sf::Color(255, 0, 0, 100),
		sf::Color(0, 255, 0, 100),
		sf::Color(0, 0, 255, 100)
};

void CityCursor::Init()
{
	engine_ = dynamic_cast<CityBuilderEngine*>(MainEngine::GetInstance());
}

void CityCursor::Update(float dt)
{
	const Index frameIndex = MainEngine::GetInstance()->frameIndex % 2;

	const auto tileSize = sf::Vector2f(engine_->GetCityMap().city.tileSize);
	auto& rect = cursorRect_[frameIndex];
	const auto mouseTilePos = GetMouseTilePos();
	rect.setPosition(sf::Vector2f(mouseTilePos.x * tileSize.x, mouseTilePos.y * tileSize.y) - tileSize / 2.0f);

	switch (cursorMode_)
	{
	case ButtonIconType::ROAD:
	{
		if (originPos_ == INVALID_TILE_POS)
		{
			rect.setSize(sf::Vector2f(tileSize));
		}
		else
		{
			const auto currentPos = GetMouseTilePos();
			const auto deltaPos = currentPos - originPos_;
			auto direction = sf::Vector2i(0, 0);
			float length;
			if (fabs(deltaPos.x) > fabs(deltaPos.y))
			{
				direction.x = int(copysign(1, deltaPos.x));
				length = fabs(float(deltaPos.x));
			}
			else
			{
				direction.y = int(copysign(1, deltaPos.y));
				length = fabs(float(deltaPos.y));
			}
			if (direction == sf::Vector2i(1, 0))
			{
				rect.setPosition(
					sf::Vector2f(originPos_.x * tileSize.x, originPos_.y * tileSize.y) - tileSize / 2.0f);
				rect.setSize(sf::Vector2f((length + 1) * tileSize.x, tileSize.y));
			}
			else if (direction == sf::Vector2i(0, 1))
			{
				rect.setPosition(
					sf::Vector2f(originPos_.x * tileSize.x, originPos_.y * tileSize.y) - tileSize / 2.0f);
				rect.setSize(sf::Vector2f(tileSize.x, (length + 1) * tileSize.y));
			}
			else if (direction == sf::Vector2i(-1, 0))
			{
				rect.setPosition(sf::Vector2f((originPos_.x - length) * tileSize.x, originPos_.y * tileSize.y) -
					tileSize / 2.0f);
				rect.setSize(sf::Vector2f((length + 1) * tileSize.x, tileSize.y));
			}
			else if (direction == sf::Vector2i(0, -1))
			{
				rect.setPosition(sf::Vector2f(originPos_.x * tileSize.x, (originPos_.y - length) * tileSize.y) -
					tileSize / 2.0f);
				rect.setSize(sf::Vector2f(tileSize.x, (length + 1) * tileSize.y));
			}
		}

		rect.setFillColor(cursorColor[Index(cursorMode_)]);
		break;
	}
	case ButtonIconType::BULLDOZER:
	case ButtonIconType::RESIDENTIAL:
	case ButtonIconType::COMMERCIAL:
	{
		if (originPos_ == INVALID_TILE_POS)
		{
			rect.setSize(sf::Vector2f(tileSize.x, tileSize.y));
		}
		else
		{
			const auto currentPos = GetMouseTilePos();
			const auto deltaPos = currentPos - originPos_;
			const sf::Vector2i direction = sf::Vector2i(
				int(copysign(1, deltaPos.x)),
				int(copysign(1, deltaPos.y)));
			rect.setPosition(sf::Vector2f(originPos_.x * tileSize.x, originPos_.y * tileSize.y) - tileSize / 2.0f);

			rect.setSize(
				sf::Vector2f(tileSize.x * float(abs(deltaPos.x) + 1), tileSize.y * float(abs(deltaPos.y) + 1)));
			if (direction.x == -1)
			{
				rect.setPosition(sf::Vector2f(currentPos.x * tileSize.x - tileSize.x / 2.0f, rect.getPosition().y));
			}
			if (direction.y == -1)
			{
				rect.setPosition(sf::Vector2f(rect.getPosition().x, currentPos.y * tileSize.y - tileSize.y / 2.0f));
			}

		}
		rect.setFillColor(cursorColor[Index(cursorMode_)]);
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
	if (event.type == sf::Event::MouseButtonPressed)
	{
		switch (event.mouseButton.button)
		{
		case sf::Mouse::Left:
			originPos_ = GetMouseTilePos();
			break;
		case sf::Mouse::Right:
			originPos_ = INVALID_TILE_POS;
			break;
		default:
			break;
		}
	}
	else if (event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left)
	{

		//Create tile road or destroy it
		switch (cursorMode_)
		{
		case ButtonIconType::ROAD:
		{
			auto& cityMap = engine_->GetCityMap();
			if (originPos_ == INVALID_TILE_POS) break;
			if (originPos_.x < 0 || originPos_.y < 0 || originPos_.x >= int(cityMap.city.mapSize.x) || originPos_.y >= int(cityMap.city.mapSize.y))
			{
				originPos_ = INVALID_TILE_POS;
				return;
			}
			const auto currentPos = GetMouseTilePos();
			if (currentPos.x < 0 || currentPos.y < 0 || currentPos.x >= int(cityMap.city.mapSize.x) || currentPos.y >= int(cityMap.city.mapSize.y))
			{
				originPos_ = INVALID_TILE_POS;
				return;
			}
			const auto deltaPos = currentPos - originPos_;
			auto direction = sf::Vector2i(0, 0);
			int length;
			if (fabs(deltaPos.x) > fabs(deltaPos.y))
			{
				direction.x = int(copysign(1, deltaPos.x));
				length = abs(deltaPos.x);
			}
			else
			{
				direction.y = int(copysign(1, deltaPos.y));
				length = abs(deltaPos.y);
			}
			for (int i = 0; i <= length; i++)
			{
				auto command = std::make_unique<BuildElementCommand>();
				command->commandType = CityCommandType::CREATE_CITY_ELEMENT;
				command->elementType = CityElementType::ROAD;
				command->position = originPos_ + direction * i;
				engine_->GetCommandManager().AddCommand(std::move(command));
			}
			originPos_ = INVALID_TILE_POS;


			break;
		}
		case ButtonIconType::BULLDOZER:
		case ButtonIconType::RESIDENTIAL:
		case ButtonIconType::COMMERCIAL:
		{
			auto& cityMap = engine_->GetCityMap();
			if (originPos_ == INVALID_TILE_POS) break;
			if (originPos_.x < 0 || originPos_.y < 0 || originPos_.x >= int(cityMap.city.mapSize.x) || originPos_.y >= int(cityMap.city.mapSize.y))
			{
				originPos_ = INVALID_TILE_POS;
				return;
			}
			const auto currentPos = GetMouseTilePos();
			if (currentPos.x < 0 || currentPos.y < 0 || currentPos.x >= int(cityMap.city.mapSize.x) || currentPos.y >= int(cityMap.city.mapSize.y))
			{
				originPos_ = INVALID_TILE_POS;
				return; 
			}
			const auto deltaPos = currentPos - originPos_;
			const auto direction = sf::Vector2i(
				int(copysign(1, deltaPos.x)),
				int(copysign(1, deltaPos.y)));
			for (int dx = 0; dx <= abs(deltaPos.x); dx++)
			{
				for (int dy = 0; dy <= abs(deltaPos.y); dy++)
				{
					switch (cursorMode_)
					{
					case ButtonIconType::BULLDOZER:
					{
						auto command = std::make_unique<DestroyElementCommand>();
						command->commandType = CityCommandType::DELETE_CITY_ELEMENT;
						command->position = originPos_ + sf::Vector2i(dx * direction.x, dy * direction.y);
						engine_->GetCommandManager().AddCommand(std::move(command));
						break;
					}
					case ButtonIconType::RESIDENTIAL:
					{
						auto command = std::make_unique<AddZoneCommand>();
						command->commandType = CityCommandType::ADD_CITY_ZONE;
						command->zoneType = ZoneType::RESIDENTIAL;
						command->position = originPos_ + sf::Vector2i(dx * direction.x, dy * direction.y);
						engine_->GetCommandManager().AddCommand(std::move(command));
						break;
					}
					case ButtonIconType::COMMERCIAL:
					{
						auto command = std::make_unique<AddZoneCommand>();
						command->commandType = CityCommandType::ADD_CITY_ZONE;
						command->zoneType = ZoneType::COMMERCIAL;
						command->position = originPos_ + sf::Vector2i(dx * direction.x, dy * direction.y);
						engine_->GetCommandManager().AddCommand(std::move(command));
						break;
					}
					default:
						break;
					}
				}
			}
			originPos_ = INVALID_TILE_POS;
			break;
		}
		default:
			break;
		}
	}
}

void CityCursor::SetCursorMode(ButtonIconType cursorMode)
{
	originPos_ = INVALID_TILE_POS;
	this->cursorMode_ = cursorMode;
	
}

sf::Vector2i CityCursor::GetMouseWorldPos() const
{
	const auto mousePos = engine_->GetMouseManager().GetPosition();
	const auto view = engine_->mainView;

	return sf::Vector2i(view.getCenter() + engine_->GetCurrentZoom() *
		(-sf::Vector2f(engine_->config.screenSize) / 2.0f + sf::Vector2f(mousePos)));

}

sf::Vector2i CityCursor::GetMouseTilePos() const
{

	const auto tileSize = engine_->GetCityMap().city.tileSize;
	const auto worldMousePos = GetMouseWorldPos() + sf::Vector2i(tileSize) / 2;
	return sf::Vector2i(
		int(worldMousePos.x) / tileSize.x,
		int(worldMousePos.y) / tileSize.y);
}
}
