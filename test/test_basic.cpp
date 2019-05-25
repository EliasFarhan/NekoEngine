#include <gtest/gtest.h>
#include <engine/engine.h>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>


TEST(Basic, OpenWindow)
{
    neko::MainEngine engine;

    engine.Init();
    engine.EngineLoop();
}

class SpriteTestEngine : public neko::MainEngine
{
public:
	void Init() override
	{
		MainEngine::Init();
		if(texture.loadFromFile("data/sprites/wall.jpg"))
		{
			sprite.setTexture(texture);
		}
	}
	void Update() override
	{
		MainEngine::Update();
		graphicsManager->Draw(sprite);
	}

private:
	sf::Texture texture;
	sf::Sprite sprite;
};

TEST(Basic, Sprite)
{
	SpriteTestEngine engine;
	engine.Init();
	engine.EngineLoop();
}