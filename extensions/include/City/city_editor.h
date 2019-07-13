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

#include <engine/editor.h>
#include "City/city_command.h"
#include "engine/globals.h"

namespace city
{

enum class ButtonIconType : neko::Index
{
	ROAD,
	BULLDOZER,
	RESIDENTIAL,
	COMMERCIAL,
	LENGTH,
	NONE
};

struct ChangeModeCommand : public CityCommand
{
	ButtonIconType newCursorMode = ButtonIconType::NONE;
};
struct ChangeTaxCommand : public CityCommand
{
	float workTax = 0.0f;
	float houseTax = 0.0f;
};

const static std::string buttonIconTexture[neko::Index(ButtonIconType::LENGTH)] =
{
	"data/sprites/icons/icons8-road-48.png",
	"data/sprites/icons/icons8-bulldozer-48.png",
	"data/sprites/icons/cottage.png",
	"data/sprites/icons/link-company-parent.png"
};

class CityEditor : public neko::Editor
{
public:
	void Init() override;
	void Update(float dt) override;
	void Destroy() override;
private:
    neko::Index buttonUiIndex[neko::Index(ButtonIconType::LENGTH)] = {};
	bool buttonSelected[neko::Index(ButtonIconType::LENGTH)] = {};
};
}
