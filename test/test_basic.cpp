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
		const auto texture = textureManager_.GetTexture(textureId);
		spriteManager_.AddComponent(entityManager_, spriteEntity_);
		spriteManager_.GetComponent(spriteEntity_).setTexture(*texture);
		
	}
	void Update(float dt) override
	{
		BasicEngine::Update(dt);
		spriteManager_.PushCommands(&graphicsManager_, 0, 1);
		graphicsManager_.Render(renderWindow.get());
	}
	void Destroy() override
	{
		BasicEngine::Destroy();
	}
protected:
	EntityManager entityManager_;
	BasicGraphicsManager graphicsManager_;
	BasicSpriteManager spriteManager_;
	TextureManager textureManager_;

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
			static_cast<float>(config.screenSize.x)); // generates random floats between 0.0 and 1.0
		std::uniform_real_distribution<float> randomY(
			0.0f,
			static_cast<float>(config.screenSize.y)); // generates random floats between 0.0 and 1.0
		std::uniform_real_distribution<float> randomScale(0.01f, 0.1f); // generates random floats between 0.0 and 1.0
		std::default_random_engine generator;
		std::uniform_real_distribution<float> randomAngle(0.0f, 360.0f); // generates random floats between 0.0 and 1.0

		for(Index i = 0; i < INIT_ENTITY_NMB; i++)
		{
			const auto entity = entityManager_.CreateEntity();

			const auto textureId = textureManager_.LoadTexture(texturePath_);
			const auto texture = textureManager_.GetTexture(textureId);

			spriteManager_.AddComponent(entityManager_, entity);
			spriteManager_.GetComponent(entity).setTexture(*texture);

			positionManager_.AddComponent(entityManager_, entity);
			positionManager_.SetComponent(entity, sf::Vector2f(randomX(generator), randomY(generator)));

			scaleManager_.AddComponent(entityManager_, entity);
			const float scale = randomScale(generator);
			scaleManager_.SetComponent(entity, sf::Vector2f(scale, scale));

			angleManager_.AddComponent(entityManager_, entity);
			angleManager_.SetComponent(entity, randomAngle(generator));
		}
		
	}
	void Update(float dt) override
	{
		BasicEngine::Update(dt);
		spriteManager_.CopyTransformPosition(positionManager_, 0, INIT_ENTITY_NMB);
		spriteManager_.CopyTransformScales(scaleManager_, 0, INIT_ENTITY_NMB);
		spriteManager_.CopyTransformAngles(angleManager_, 0, INIT_ENTITY_NMB);

		spriteManager_.PushCommands(&graphicsManager_, 0, INIT_ENTITY_NMB);

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
	BasicSpriteManager spriteManager_;
	BasicGraphicsManager graphicsManager_;
	const std::string texturePath_ = "data/sprites/wall.jpg";

};

TEST(Basic, TransformSpriteTest)
{
	TransformSpriteTestEngine engine;
	engine.Init();
	engine.EngineLoop();
}
}
