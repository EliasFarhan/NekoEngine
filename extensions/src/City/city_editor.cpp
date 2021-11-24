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
#include <imgui.h>
#include <imgui-SFML.h>
#include "engine/engine.h"
#include <City/city_engine.h>

#ifdef TRACY_ENABLE
#include <Tracy.hpp>
#endif
namespace neko
{
void CityEditor::Init()
{

#ifdef TRACY_ENABLE
	ZoneScoped
#endif
	engine_ = dynamic_cast<CityBuilderEngine*>(MainEngine::GetInstance());
	for (Index i = 0; i < Index(ButtonIconType::LENGTH); i++)
	{
		buttonUiIndex[i] = engine_->GetTextureManager().LoadTexture(buttonIconTexture[i]);
	}
}

void CityEditor::Update(float dt)
{

#ifdef TRACY_ENABLE
	ZoneScoped
#endif
	const Index frameIndex = (engine_->frameIndex - 1) % 2; //Render frame is always the previous one
	ImGui::SFML::Update(*renderWindow_, engine_->clockDeltatime);

	ImGui::Begin("Inspector");
	//Draw inspector data
	for (auto& inspectorData : inspectorValues_[frameIndex])
	{
		ImGui::LabelText(inspectorData.first.c_str(), "%s", inspectorData.second.c_str());
	}
	ImGui::End();


	ImGui::Begin("Gameplay");
	for (Index i = 0; i < static_cast<Index>(ButtonIconType::LENGTH); i++)
	{
		const auto buttonTexture = engine_->GetTextureManager().GetTexture(buttonUiIndex[i]);
		if (ImGui::ImageButton(*buttonTexture))
		{
			std::fill(std::begin(buttonSelected), std::end(buttonSelected), false);
			buttonSelected[i] = true;

			auto newCommand = std::make_unique<ChangeModeCommand>();
			newCommand->newCursorMode = static_cast<ButtonIconType>(i);
			newCommand->commandType = CityCommandType::CHANGE_CURSOR_MODE;
			engine_->GetCommandManager().AddCommand(std::move(newCommand), true);
		}
	}
	//Tax management
	{
		int newWorkTax = int(100.0f * engine_->workTax);
		ImGui::SliderInt("Work Tax", &newWorkTax, 0, 50, "%d%");
		int newHouseTax = int(100.0f * engine_->houseTax);
		ImGui::SliderInt("House Tax", &newHouseTax, 0, 50, "%d%");
		if (newWorkTax != int(100.0f * engine_->workTax) || newHouseTax != int(100.0f * engine_->houseTax))
		{
			auto newCommand = std::make_unique<ChangeTaxCommand>();
			newCommand->workTax = float(newWorkTax) / 100.0f;
			newCommand->houseTax = float(newHouseTax) / 100.0f;
			newCommand->commandType = CityCommandType::CHANGE_TAX;
			engine_->GetCommandManager().AddCommand(std::move(newCommand), true);
		}
	}
	ImGui::End();
	inspectorValues_[frameIndex].clear();

	ImGui::SFML::Render(*renderWindow_);
}

void CityEditor::Destroy()
{
}
}
