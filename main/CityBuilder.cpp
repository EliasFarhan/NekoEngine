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
#include <string>
#include <engine/engine.h>
#include <city_tilemap.h>
#include <city_map.h>
#include "engine/input.h"

#include <SFML/Window/Event.hpp>
#include "engine/log.h"

class CityBuilderEngine : public neko::MainEngine
{
public:

	

	void Init() override
	{
	    config.vSync = true;
		MainEngine::Init();
		cityBuilderMap.Init();
		environmentTilemap.Init(textureManager);
		mainView = renderWindow->getView();
	}

	void Update() override
	{
		MainEngine::Update();

		if(mouseManager_.IsButtonPressed(sf::Mouse::Button::Middle))
		{
			const auto delta = sf::Vector2f(mouseManager_.GetMouseDelta());
			mainView.setCenter(mainView.getCenter() - currentZoom*delta);

		}
        environmentTilemap.UpdateTilemap(cityBuilderMap, mainView, neko::CityTilesheetType::LENGTH);
		environmentTilemap.PushCommand(graphicsManager_.get());
		graphicsManager_->SetView(mainView);
		graphicsManager_->editor.AddInspectorInfo("FPS", std::to_string(1.0f/dt.asSeconds()));
	}

	void OnEvent(sf::Event& event) override
	{
		MainEngine::OnEvent(event);
		if(event.type == sf::Event::MouseWheelScrolled)
		{
			const float wheelDelta = event.mouseWheelScroll.delta;
			{
				std::ostringstream oss;
				oss << "Mouse Wheel Delta: " << wheelDelta;
				logDebug(oss.str());
			}
			const auto size = mainView.getSize();
			currentZoom -= wheelDelta * scrollDelta * currentZoom;

			mainView.setSize(size - wheelDelta * scrollDelta * size);
		}
	}

	void Destroy() override
	{
		MainEngine::Destroy();
	}

    sf::View mainView;
private:
	neko::EntityManager entityManager;
	neko::TextureManager textureManager;
	neko::CityBuilderTilemap environmentTilemap;
	neko::CityBuilderMap cityBuilderMap;

	const float scrollDelta = 0.1f;
	float currentZoom = 1.0f;
};


int main([[maybe_unused]] int argc, [[maybe_unused]] char** argv)
{
	CityBuilderEngine engine;
	engine.Init();
	engine.EngineLoop();
	return EXIT_SUCCESS;
}
