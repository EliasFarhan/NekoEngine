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
#include <SFML/Graphics/RectangleShape.hpp>
#include "engine/system.h"
#include "city/editor.h"

namespace neko
{
class CityBuilderEngine;

class CityCursor : public System
{
public:
	void Init() override;
	void Update(float dt) override;
	void Destroy() override;
	void OnEvent(sf::Event& event);
	void SetCursorMode(ButtonIconType cursorMode);
	sf::Vector2i GetMouseWorldPos() const;
	sf::Vector2i GetMouseTilePos() const;
	CityBuilderEngine* engine_;
protected:

	ButtonIconType cursorMode_ = ButtonIconType::NONE;
	//for double buffering
	sf::RectangleShape cursorRect_[2];
	sf::Vector2i originPos_ =sf::Vector2i(-1,-1);
};
}
