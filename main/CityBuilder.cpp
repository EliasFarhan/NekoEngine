
#include <sstream>
#include <engine/engine.h>
#include <city_tilemap.h>
#include "engine/input.h"

#include <SFML/Window/Event.hpp>
#include "engine/log.h"

class CityBuilderEngine : public neko::MainEngine
{
public:

	void Init() override
	{
		MainEngine::Init();
		environmentTilemap.Init(textureManager);
		mainView = renderWindow->getView();
	}

	void Update() override
	{
		MainEngine::Update();
		environmentTilemap.PushCommand(graphicsManager);
		graphicsManager->SetView(mainView);
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
			mainView.setSize(size - wheelDelta * scrollDelta * size);
		}
	}

	void Destroy() override
	{
		MainEngine::Destroy();
	}

private:
	sf::View mainView;
	neko::EntityManager entityManager;
	neko::TextureManager textureManager;
	neko::CityBuilderTilemap environmentTilemap;

	const float scrollDelta = 0.1f;

};


int main(int argc, char** argv)
{
	(void)argc;
	(void)argv;
	CityBuilderEngine engine;
	engine.Init();
	engine.EngineLoop();
	return EXIT_SUCCESS;
}