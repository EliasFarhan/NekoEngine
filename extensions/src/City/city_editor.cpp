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

#include <City/city_editor.h>
#include <imgui-SFML.h>
#include <imgui.h>
#include "engine/engine.h"
#include <City/city_engine.h>
#include <graphics/texture.h>

namespace city
{
void CityEditor::Init()
{
	auto* engine = neko::BasicEngine::GetInstance<CityBuilderEngine>();
	for (neko::Index i = 0; i < neko::Index(ButtonIconType::LENGTH); i++)
	{
		buttonUiIndex[i] = engine->GetTextureManager().LoadTexture(buttonIconTexture[i]);
	}
}

void CityEditor::Update(float dt)
{
	const neko::Index frameIndex = (multi::MainEngine::GetFrameIndex()- 1) % 2; //Render frame is always the previous one
	auto* engine = neko::BasicEngine::GetInstance<CityBuilderEngine>();
	auto* renderWindow = engine->renderWindow.get();
	ImGui::SFML::Update(*renderWindow, engine->clockDeltatime);

	ImGui::Begin("Inspector");
	//Draw inspector data
	for (auto& inspectorData : inspectorValues_[frameIndex])
	{
		ImGui::LabelText(inspectorData.first.c_str(), "%s", inspectorData.second.c_str());
	}
	ImGui::End();


	ImGui::Begin("Gameplay");
	for (neko::Index i = 0; i < neko::Index(ButtonIconType::LENGTH); i++)
	{
		const auto buttonTexture = engine->GetTextureManager().GetTexture(buttonUiIndex[i]);
		if (ImGui::ImageButton(*buttonTexture))
		{
			std::fill(std::begin(buttonSelected), std::end(buttonSelected), false);
			buttonSelected[i] = true;

			auto newCommand = std::make_unique<ChangeModeCommand>();
			newCommand->newCursorMode = ButtonIconType(i);
			newCommand->commandType = CityCommandType::CHANGE_CURSOR_MODE;
			engine->GetCommandManager().AddCommand(std::move(newCommand), true);
		}
	}
	//Tax management
	{
		int newWorkTax = int(100.0f * engine->workTax);
		ImGui::SliderInt("Work Tax", &newWorkTax, 0, 50, "%d%");
		int newHouseTax = int(100.0f * engine->houseTax);
		ImGui::SliderInt("House Tax", &newHouseTax, 0, 50, "%d%");
		if (newWorkTax != int(100.0f * engine->workTax) || newHouseTax != int(100.0f * engine->houseTax))
		{
			auto newCommand = std::make_unique<ChangeTaxCommand>();
			newCommand->workTax = float(newWorkTax) / 100.0f;
			newCommand->houseTax = float(newHouseTax) / 100.0f;
			newCommand->commandType = CityCommandType::CHANGE_TAX;
			engine->GetCommandManager().AddCommand(std::move(newCommand), true);
		}
	}
	ImGui::End();
	inspectorValues_[frameIndex].clear();

	ImGui::SFML::Render(*renderWindow);
}

void CityEditor::Destroy()
{
}
}
