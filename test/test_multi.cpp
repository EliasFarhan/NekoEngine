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
#include <graphics/tilemap.h>
#include "utilities/time_utility.h"
#include <multi/engine/engine.h>
#include <multi/graphics/graphics.h>
#include <multi/graphics/sprite.h>
#include <multi/graphics/tilemap.h>

namespace multi
{

TEST(Multi, OpenWindow)
{
    MainEngine engine;

    engine.Init();
    engine.EngineLoop();
}

class SpriteTestEngine : public MainEngine
{
public:
    void Init() override
    {
        MainEngine::Init();
		const neko::Index textureIndex = 
			textureManager.LoadTexture("data/sprites/wall.jpg");
		texture = textureManager.GetTexture(textureIndex);
        sprite.setTexture(*texture);

    }

    void Update(float dt) override
    {
        MainEngine::Update(dt);
        graphicsManager_->Draw(sprite);

    }

private:
    neko::TextureManager textureManager;
    std::shared_ptr<sf::Texture> texture;
    sf::Sprite sprite;
};

TEST(Multi, Sprite)
{
    SpriteTestEngine engine;
    engine.Init();
    engine.EngineLoop();
}

class MovingSpriteEngine : public multi::MainEngine
{
public:
    void Init() override
    {
        MainEngine::Init();
		const neko::Index textureIndex = 
			textureManager_.LoadTexture("data/sprites/wall.jpg");
		const std::shared_ptr<sf::Texture> texture = textureManager_.GetTexture(textureIndex);

        std::uniform_real_distribution<float> randomX(
			0.0f,
			static_cast<float>(config.screenSize.x)); // generates random floats between 0.0 and 1.0
        std::uniform_real_distribution<float> randomY(
			0.0f,
			static_cast<float>(config.screenSize.y)); // generates random floats between 0.0 and 1.0
        std::uniform_real_distribution<float> randomScale(0.01f, 0.1f); // generates random floats between 0.0 and 1.0
        std::default_random_engine generator;
        std::uniform_real_distribution<float> randomAngle(0.0f, 360.0f); // generates random floats between 0.0 and 1.0
        for (auto i = 0u; i < neko::INIT_ENTITY_NMB; i++)
        {
            const auto entity = entityManager_.CreateEntity();
            spriteManager_.AddSprite(texture);

            positionManager_.AddComponent(entityManager_, entity);
            positionManager_.SetComponent(entity, sf::Vector2f(randomX(generator), randomY(generator)));
            const float scale = randomScale(generator);
            scaleManager_.AddComponent(entityManager_, entity);
            scaleManager_.SetComponent(entity, sf::Vector2f(scale, scale));
            angleManager_.AddComponent(entityManager_, entity);
            angleManager_.SetComponent(entity, randomAngle(generator));

        }

    }

    void Update(float dt) override
    {
        MainEngine::Update(dt);
        spriteManager_.CopyTransformPosition(positionManager_, 0, neko::INIT_ENTITY_NMB);
        spriteManager_.CopyTransformScales(scaleManager_, 0, neko::INIT_ENTITY_NMB);
        spriteManager_.CopyTransformAngles(angleManager_, 0, neko::INIT_ENTITY_NMB);
        spriteManager_.PushCommands(graphicsManager_.get());
    }

protected:
    neko::TextureManager textureManager_;
    multi::SpriteManager spriteManager_;
    neko::EntityManager entityManager_;
    neko::Position2dManager positionManager_;
    neko::Scale2dManager scaleManager_;
    neko::Angle2dManager angleManager_;
};

TEST(Multi, MovingSprite)
{
    MovingSpriteEngine engine;
    engine.Init();
    engine.EngineLoop();
}

class SpriteAnimatorEngine : public MainEngine
{
public:
    void Init() override
    {
        config.vSync = true;
        config.bgColor = sf::Color::White;
        MainEngine::Init();

        {
            const auto entity = entityManager_.CreateEntity();
            positionManager_.AddComponent(entityManager_, entity);
            positionManager_.SetComponent(entity, sf::Vector2f(renderWindow->getSize()) / 2.0f);
            auto spriteIndex = spriteManager_.AddSprite(nullptr);
            auto& animator = animatorManager_.CreateSpriteAnimator();

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
                            {"rects", {0, 0, 32, 32}},
                            {"value", 0}
                    }
            };
            json animWalkJson;
            animWalkJson["name"] = "walk";
            animWalkJson["freq"] = 12;
            animWalkJson["keys"] = {
                    {
                            {"rects", {0, 0, 32, 32}},
                            {"value", 1}
                    },
                    {
                            {"rects", {0, 0, 32, 32}},
                            {"value", 2}
                    },
                    {
                            {"rects", {0, 0, 32, 32}},
                            {"value", 3}
                    },
            };
            json animIdleJson;
            animIdleJson["name"] = "idle";
            animIdleJson["freq"] = 12;
            animIdleJson["keys"] = {
                    {
                            {"rects", {0, 0, 32, 32}},
                            {"value", 4}
                    }
            };
            animatorJson["anims"] = {
                    animIdleJson, animJumpJson, animWalkJson
            };
            animator.Init(animatorJson, textureManager_, spriteIndex);
        }

    }

    void Update(float dt) override
    {
        MainEngine::Update(dt);
        if (keyboardManager_.IsKeyDown(sf::Keyboard::Up))
        {
            auto* animator = animatorManager_.GetAnimatorAt(0);
            animator->PlayAnim("jump");
        }
        if (keyboardManager_.IsKeyDown(sf::Keyboard::Down))
        {
            auto* animator = animatorManager_.GetAnimatorAt(0);
            animator->PlayAnim("idle");
        }
        if (keyboardManager_.IsKeyDown(sf::Keyboard::Right))
        {
            auto* animator = animatorManager_.GetAnimatorAt(0);
            animator->PlayAnim("walk");
        }
        animatorManager_.Update(spriteManager_, dt);
        spriteManager_.CopyTransformPosition(positionManager_, 0, 1);
        spriteManager_.PushCommands(graphicsManager_.get(), 0, 1);
    }

    void Destroy() override
    {
        MainEngine::Destroy();
    }


protected:
    neko::AnimatorManager animatorManager_;
    neko::TextureManager textureManager_;
    multi::SpriteManager spriteManager_;
    neko::EntityManager entityManager_;
    neko::Position2dManager positionManager_;
};

TEST(Multi, SpriteAnimator)
{
    SpriteAnimatorEngine engine;
    engine.Init();
    engine.EngineLoop();
}


class TilemapEngine : public MainEngine
{
public:
    void Init() override
    {
        MainEngine::Init();
        tiledMap.Init("data/tilemap/platformer.json", textureManager);
    }

    void Update(float dt) override
    {
        MainEngine::Update(dt);
        tiledMap.PushCommand(graphicsManager_.get());
    }

    void Destroy() override
    {
        MainEngine::Destroy();
    }

protected:
    neko::TextureManager textureManager;
    multi::TiledMap tiledMap;
};

TEST(Multi, Tilemap)
{
    TilemapEngine engine;
    engine.Init();
    engine.EngineLoop();
}
}