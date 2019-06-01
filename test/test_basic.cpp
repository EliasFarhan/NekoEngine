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
#include <graphics/anim.h>
#include <engine/input.h>
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

			spriteManager.AddSprite(texture);

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

class SpriteAnimatorEngine : public neko::MainEngine
{
public:
    void Init() override
    {
        config.vSync = true;
        config.bgColor = sf::Color::White;
        MainEngine::Init();

        {
            transformManager.positions.push_back(sf::Vector2f(renderWindow->getSize())/2.0f);
            auto spriteIndex = spriteManager.AddSprite(nullptr);
            auto& animator = animatorManager.CreateSpriteAnimator();

            json animatorJson;

            animatorJson["textures"] =
            {
                "data/sprites/hero/jump/hero1.png",
                "data/sprites/hero/move/hero1.png",
                "data/sprites/hero/move/hero2.png",
                "data/sprites/hero/move/hero3.png",
                "data/sprites/hero/still/hero1.png",
            };
            json animJumpJson;
            animJumpJson["name"] = "jump";
            animJumpJson["freq"] = 12;
            animJumpJson["keys"] = {
                {
                    {"rects", {0,0,32,32}},
                    {"value", 0}
                }
            };
            json animWalkJson;
            animWalkJson["name"] = "walk";
            animWalkJson["freq"] = 12;
            animWalkJson["keys"] = {
                {
                    {"rects", {0,0,32,32}},
                    {"value", 1}
                },
                {
                    {"rects", {0,0,32,32}},
                    {"value", 2}
                },
                {
                    {"rects", {0,0,32,32}},
                    {"value", 3}
                },
            };
            json animIdleJson;
            animIdleJson["name"] = "idle";
            animIdleJson["freq"] = 12;
            animIdleJson["keys"] = {
            {
                {"rects", {0,0,32,32}},
                {"value", 4}
            }
            };
            animatorJson["anims"] = {
                    animIdleJson, animJumpJson, animWalkJson
            };
            animator.Init(animatorJson, textureManager, spriteIndex);
        }

    }

    void Update() override
    {
        MainEngine::Update();
        keyboardManager.Update();
        if(keyboardManager.IsKeyDown(sf::Keyboard::Up))
        {
            auto* animator = animatorManager.GetAnimatorAt(0);
            animator->PlayAnim("jump");
        }
        if(keyboardManager.IsKeyDown(sf::Keyboard::Down))
        {
            auto* animator = animatorManager.GetAnimatorAt(0);
            animator->PlayAnim("idle");
        }
        if(keyboardManager.IsKeyDown(sf::Keyboard::Right))
        {
            auto* animator = animatorManager.GetAnimatorAt(0);
            animator->PlayAnim("walk");
        }
        animatorManager.Update(spriteManager, dt.asSeconds());
        spriteManager.CopyTransformPosition(transformManager, 0,1);
        spriteManager.PushCommands(graphicsManager, 0, 1);
    }

    void Destroy() override
    {
        MainEngine::Destroy();
    }


protected:
    neko::KeyboardManager keyboardManager;
    neko::AnimatorManager animatorManager;
    neko::TextureManager textureManager;
    neko::SpriteManager spriteManager;
    neko::Transform2dManager transformManager;
};

TEST(Basic, SpriteAnimator)
{
    SpriteAnimatorEngine engine;
    engine.Init();
    engine.EngineLoop();
}