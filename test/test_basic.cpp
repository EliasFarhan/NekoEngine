//
// Created by efarhan on 13.07.19.
//

#include <gtest/gtest.h>
#include "engine/engine.h"
#include <graphics/sprite.h>
#include <random>

namespace neko
{
TEST(Basic, OpenWindow)
{
	neko::BasicEngine engine;
	engine.Init();
	engine.EngineLoop();
}

class SpriteTestEngine : public BasicEngine
{
public:
	void Init() override
	{
		BasicEngine::Init();
		spriteEntity_ = entityManager_.CreateEntity();
		const auto textureId = textureManager_.LoadTexture(texturePath_);
		spriteManager_.AddComponent(entityManager_, spriteEntity_);
        spriteManager_.CopyTexture(textureId, spriteEntity_, 1);
		
	}
	void Update(float dt) override
	{
		BasicEngine::Update(dt);
		spriteManager_.PushAllCommands(entityManager_, graphicsManager_);
		graphicsManager_.Render(renderWindow.get());
	}
	void Destroy() override
	{
		BasicEngine::Destroy();
	}
protected:
	EntityManager entityManager_;
	GraphicsManager graphicsManager_;

    TextureManager textureManager_;
	SpriteManager spriteManager_{this->textureManager_};

	Entity spriteEntity_ = INVALID_ENTITY;

	const std::string texturePath_ = "data/sprites/wall.jpg";
};

TEST(Basic, ShowSprite)
{
	SpriteTestEngine engine;
	engine.Init();
	engine.EngineLoop();
}


class TransformSpriteTestEngine : public BasicEngine
{
public:
	void Init() override
	{
		BasicEngine::Init();
		std::uniform_real_distribution<float> randomX(
			0.0f,
			static_cast<float>(config.realWindowSize.x)); // generates random floats between 0.0 and 1.0
		std::uniform_real_distribution<float> randomY(
			0.0f,
			static_cast<float>(config.realWindowSize.y)); // generates random floats between 0.0 and 1.0
		std::uniform_real_distribution<float> randomScale(0.01f, 0.1f); // generates random floats between 0.0 and 1.0
		std::default_random_engine generator;
		std::uniform_real_distribution<float> randomAngle(0.0f, 360.0f); // generates random floats between 0.0 and 1.0
        const auto textureId = textureManager_.LoadTexture(texturePath_);
		for(Index i = 0; i < INIT_ENTITY_NMB; i++)
		{
			const auto entity = entityManager_.CreateEntity(i);



			spriteManager_.AddComponent(entityManager_, entity);

			positionManager_.AddComponent(entityManager_, entity);
			positionManager_.SetComponent(entity, sf::Vector2f(randomX(generator), randomY(generator)));

			scaleManager_.AddComponent(entityManager_, entity);
			const float scale = randomScale(generator);
			scaleManager_.SetComponent(entity, sf::Vector2f(scale, scale));

			angleManager_.AddComponent(entityManager_, entity);
			angleManager_.SetComponent(entity, randomAngle(generator));
		}
		spriteManager_.CopySpriteOrigin(sf::Vector2f(0.5f,0.5f),0, INIT_ENTITY_NMB);
        spriteManager_.CopyTexture(textureId, 0, INIT_ENTITY_NMB);
		
	}
	void Update(float dt) override
	{
		BasicEngine::Update(dt);
		spriteManager_.CopyAllTransformPositions(entityManager_, positionManager_);
		spriteManager_.CopyAllTransformScales(entityManager_, scaleManager_);
		spriteManager_.CopyAllTransformAngles(entityManager_, angleManager_);

		spriteManager_.PushAllCommands(entityManager_, graphicsManager_);

		graphicsManager_.Render(renderWindow.get());
	}
	void Destroy() override
	{
		BasicEngine::Destroy();
	}
protected:
	EntityManager entityManager_;
	Position2dManager positionManager_;
	Scale2dManager scaleManager_;
	Angle2dManager angleManager_;
	TextureManager textureManager_;
	SpriteManager spriteManager_{this->textureManager_};
	GraphicsManager graphicsManager_;
	const std::string texturePath_ = "data/sprites/wall.jpg";

};

TEST(Basic, TransformSpriteTest)
{
	TransformSpriteTestEngine engine;
	engine.Init();
	engine.EngineLoop();
}
}
