#include <gtest/gtest.h>
#include <engine/engine.h>
#include <SFML/Graphics/Sprite.hpp>
#include <SFML/Graphics/Texture.hpp>
#include "engine/log.h"
#include "graphics/texture.h"
#include "graphics/sprite.h"
#include "engine/transform.h"
#include "engine/globals.h"
#include <random>
#include "utilities/time_utility.h"


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
		texture = textureManager.LoadTexture("data/sprites/wall.jpg");
		sprite.setTexture(*texture);
		
	}
	void Update() override
	{
		MainEngine::Update();
		graphicsManager->Draw(sprite);
		
	}

private:
	neko::TextureManager textureManager;
	sf::Texture* texture;
	sf::Sprite sprite;
};

TEST(Basic, Sprite)
{
	SpriteTestEngine engine;
	engine.Init();
	engine.EngineLoop();
}

class MovingSpriteEngine : public neko::MainEngine
{
public:
	void Init() override
	{
		MainEngine::Init();
		const auto* texture = textureManager.LoadTexture("data/sprites/wall.jpg");

		std::uniform_real_distribution<float> randomX(0.0f,config.screenSize.x); // generates random floats between 0.0 and 1.0
		std::uniform_real_distribution<float> randomY(0.0f,config.screenSize.y); // generates random floats between 0.0 and 1.0
		std::uniform_real_distribution<float> randomScale(0.01f,0.1f); // generates random floats between 0.0 and 1.0
		std::default_random_engine generator;
		std::uniform_real_distribution<float> randomAngle(0.0f, 360.0f); // generates random floats between 0.0 and 1.0
		for(auto i = 0u; i < InitEntityNmb; i++)
		{
			sf::Sprite sprite;
			sprite.setTexture(*texture);
			sprite.setOrigin(sf::Vector2f(
				texture->getSize().x * 0.5f,
				texture->getSize().y * 0.5f
			));
			spriteManager.AddSprite(sprite);

			transformManager.positions.emplace_back(randomX(generator), randomY(generator));
			const float scale = randomScale(generator);
			transformManager.scales.emplace_back(scale, scale);
			transformManager.angles.push_back(randomAngle(generator));

		}

	}
	void Update() override
	{
		MainEngine::Update();
		spriteManager.CopyTransformPosition(transformManager);
		spriteManager.CopyTransformScales(transformManager);
		spriteManager.CopyTransformAngles(transformManager);
		spriteManager.PushCommands(graphicsManager);
	}

protected:
	neko::TextureManager textureManager;
	neko::SpriteManager spriteManager;
	neko::Transform2dManager transformManager;
};

TEST(Basic, MovingSprite)
{
	MovingSpriteEngine engine;
	engine.Init();
	engine.EngineLoop();
}