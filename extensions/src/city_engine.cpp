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
#include <sstream>
#include <city_engine.h>
#include "city_editor.h"

#include <engine/log.h>
namespace neko
{
void CityBuilderEngine::Init()
{
	config.vSync = true;
	MainEngine::Init();
	cityBuilderMap_.Init();
	environmentTilemap_.Init(textureManager_);
	mainView = renderWindow->getView();
	graphicsManager_->editor = std::make_unique<CityEditor>();
	graphicsManager_->editor->Init();

	cursor_.Init();
	commandManager_.Init();
}

void CityBuilderEngine::Update()
{
	MainEngine::Update();

	
	commandManager_.Update();
	if (mouseManager_.IsButtonPressed(sf::Mouse::Button::Middle))
	{
		const auto delta = sf::Vector2f(mouseManager_.GetMouseDelta());
		mainView.setCenter(mainView.getCenter() - currentZoom_ * delta);

	}
	environmentTilemap_.UpdateTilemap(cityBuilderMap_, mainView, CityTilesheetType::LENGTH);
	environmentTilemap_.PushCommand(graphicsManager_.get());
	cursor_.Update();
	graphicsManager_->SetView(mainView);
	graphicsManager_->editor->AddInspectorInfo("FPS", std::to_string(1.0f / dt.asSeconds()));
}

void CityBuilderEngine::OnEvent(sf::Event& event)
{
	MainEngine::OnEvent(event);
	cursor_.OnEvent(event);
	if (event.type == sf::Event::MouseWheelScrolled)
	{
		const float wheelDelta = event.mouseWheelScroll.delta;
		{
			std::ostringstream oss;
			oss << "Mouse Wheel Delta: " << wheelDelta;
			logDebug(oss.str());
		}
		const auto size = mainView.getSize();
		currentZoom_ -= wheelDelta * scrollDelta_ * currentZoom_;

		mainView.setSize(size - wheelDelta * scrollDelta_ * size);
	}
}

void CityBuilderEngine::Destroy()
{
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
}
