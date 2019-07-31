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
#include <multi/graphics/shape.h>

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
		texture = *textureManager.GetTexture(textureIndex);
        sprite.setTexture(texture);

    }

    void Update(float dt) override
    {
        MainEngine::Update(dt);
        graphicsManager_->Draw(sprite);

    }

private:
    neko::TextureManager textureManager;
    sf::Texture texture;
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
		const auto* texture = textureManager_.GetTexture(textureIndex);

        std::uniform_real_distribution<float> randomX(
			0.0f,
			static_cast<float>(config.realWindowSize.x)); // generates random floats between 0.0 and 1.0
        std::uniform_real_distribution<float> randomY(
			0.0f,
			static_cast<float>(config.realWindowSize.y)); // generates random floats between 0.0 and 1.0
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

struct PlayerData
{
    neko::Entity playerEntity;
    int contactNmb;
    b2Body* playerBody = nullptr;
};

class PlatformerEngine : public multi::MainEngine
{
public:
    void Init() override
    {
        config.bgColor = sf::Color::White;
        MainEngine::Init();
        physicsManager.Init();
        {
            playerData.playerEntity = entityManager.CreateEntity();
            const auto playerPos = sf::Vector2f(200, 200);

            positionManager.AddComponent(entityManager, playerData.playerEntity);
            positionManager.SetComponent(playerData.playerEntity, playerPos);

            const neko::Index textureIndex = textureManager.LoadTexture("data/sprites/hero/jump/hero1.png");
            auto* texture = textureManager.GetTexture(textureIndex);
            spriteManager.AddSprite(texture);

            const auto physicsSize = neko::pixel2meter(sf::Vector2f(texture->getSize()));
            b2BodyDef bodyDef;
            bodyDef.position = neko::pixel2meter(playerPos);
            bodyDef.linearVelocity = b2Vec2(0,0);
            bodyDef.fixedRotation = true;
            bodyDef.type = b2_dynamicBody;

            b2PolygonShape playerBox[2];
            playerBox[0].SetAsBox(physicsSize.x/2.0f, physicsSize.y/2.0f);

            const float footOffset = 5.0f;
            const b2Vec2 footPoints[3] = {
                    b2Vec2(0,physicsSize.y/2.0f),
                    b2Vec2(neko::pixel2meter(footOffset),physicsSize.y/2.0f + neko::pixel2meter(footOffset)),
                    b2Vec2(neko::pixel2meter(-footOffset),physicsSize.y/2.0f + neko::pixel2meter(footOffset)),
            };
            playerBox[1].Set(footPoints, 3);

            b2FixtureDef fixtureDef[2];

            neko::Collider mainPlayerCollider{};
#ifdef __neko_dbg__
            neko::ShapeDef shapeDef;
			shapeDef.fillColor = sf::Color::Transparent;
			shapeDef.outlineColor = sf::Color::Green;
			shapeDef.outlineThickness = 1.0f;
			auto mainPlayerShapeIndex = shapeManager.AddBox(playerPos, neko::meter2pixel(b2Vec2(physicsSize.x/2.0f, physicsSize.y/2.0f)), shapeDef);
			mainPlayerCollider.shapeIndex = mainPlayerShapeIndex;
#endif
            fixtureDef[0].shape = &playerBox[0];
            fixtureDef[0].friction = 0.0f;
            mainPlayerCollider.entity = playerData.playerEntity;
            physicsManager.AddCollider(mainPlayerCollider);

            neko::Collider footCollider{};
            footCollider.entity = playerData.playerEntity;
            fixtureDef[1].shape = &playerBox[1];
            fixtureDef[1].isSensor = true;
#ifdef __neko_dbg__

            sf::Vector2f points[3];
			for(int i = 0; i < 3; i++)
            {
			    points[i] = neko::meter2pixel(footPoints[i]);
            }

            //auto footShapeIndex = shapeManager.AddPolygon(neko::meter2pixel(footPoints[0]), points, 3, shapeDef);
            //footCollider.shapeIndex = footShapeIndex;
#endif
            physicsManager.AddCollider(footCollider);

            playerData.playerBody = physicsManager.CreateBody(bodyDef, fixtureDef, 2);
        }
        {
            const sf::Vector2f platformPositions[PlatformerEngine::platformsNmb] = {
                    sf::Vector2f(200,400),
                    sf::Vector2f(300,300),
                    sf::Vector2f(300,500)
            };
            const neko::Index textureIndex =
                    textureManager.LoadTexture("data/sprites/platform.jpg");
            auto* platformTexture = textureManager.GetTexture(textureIndex);

            for (auto i = 0u ; i < platformsNmb;i++)
            {
                const auto platformEntity = entityManager.CreateEntity();
                positionManager.AddComponent(entityManager, platformEntity);
                positionManager.SetComponent(platformEntity, platformPositions[i]);

                const auto physicsSize = neko::pixel2meter(sf::Vector2f(platformTexture->getSize()));
                b2BodyDef bodyDef;
                bodyDef.position = neko::pixel2meter(platformPositions[i]);
                bodyDef.fixedRotation = true;
                bodyDef.type = b2_staticBody;

                b2PolygonShape playerBox;
                playerBox.SetAsBox(physicsSize.x / 2.0f, physicsSize.y / 2.0f);

                b2FixtureDef fixtureDef;
                fixtureDef.shape = &playerBox;

                neko::Collider platformCollider{};
                platformCollider.entity = playerData.playerEntity + i + 1;
#ifdef __neko_dbg__
                neko::ShapeDef shapeDef;
                shapeDef.fillColor = sf::Color::Transparent;
                shapeDef.outlineColor = sf::Color::Green;
                shapeDef.outlineThickness = 1.0f;
                auto platformShapeIndex = shapeManager.AddBox(platformPositions[i], neko::meter2pixel(b2Vec2(physicsSize.x/2.0f, physicsSize.y/2.0f)), shapeDef);
                platformCollider.shapeIndex = platformShapeIndex;
#endif
                physicsManager.AddCollider(platformCollider);
                physicsManager.CreateBody(bodyDef, &fixtureDef, 1);
                spriteManager.AddSprite(platformTexture);
            }
        }
        physicsTimer.period = config.physicsTimeStep;
        physicsTimer.Reset();

    }




    void Update(float dt) override
    {

        MainEngine::Update(dt);
        physicsTimer.Update(dt);
        if(physicsTimer.IsOver())
        {
            physicsManager.Update(dt);
            physicsTimer.time += physicsTimer.period;
        }
        //Player management
        {
            //Jumping
            if(playerData.contactNmb > 0 && keyboardManager_.IsKeyDown(sf::Keyboard::Space))
            {
                const auto playerVelocity = playerData.playerBody->GetLinearVelocity();
                playerData.playerBody->SetLinearVelocity(b2Vec2(playerVelocity.x, -jumpVelocity));
            }
            //Move horizontal
            {
                int move = 0;
                move -= keyboardManager_.IsKeyHeld(sf::Keyboard::Left);
                move += keyboardManager_.IsKeyHeld(sf::Keyboard::Right);
                {
                    std::ostringstream oss;
                    oss << "Player Key Move: "<<move;
                    logDebug(oss.str());
                }
                const auto playerVelocity = playerData.playerBody->GetLinearVelocity();
                playerData.playerBody->SetLinearVelocity(b2Vec2(move*moveVelocity, playerVelocity.y));
            }

            positionManager.CopyPositionsFromPhysics2d(entityManager, physicsManager);
            spriteManager.CopyTransformPosition(positionManager, 0,1);
            spriteManager.PushCommands(graphicsManager_.get(), 0,1);
        }


        {
            spriteManager.CopyTransformPosition(positionManager, 1, platformsNmb);
            spriteManager.PushCommands(graphicsManager_.get(), 1,platformsNmb);
        }
#ifdef __neko_dbg__
        {

            //Main playerbox
            shapeManager.CopyTransformPosition(positionManager, 0, 1);
            shapeManager.PushCommands(graphicsManager_.get(), 0, 1+platformsNmb);
        }
        {
            std::ostringstream oss;
            auto pos = transformManager.GetComponent(playerData.playerEntity);
            oss << "("<<pos.x<<", "<<pos.y<<")";
            graphicsManager_->editor->AddInspectorInfo("PlayerPos", oss.str());
        }
        {
            std::ostringstream oss;
            auto* body = physicsManager.GetBodyAt(playerData.playerEntity);
            b2Vec2 velocity = body->GetLinearVelocity();
            oss << "("<<velocity.x<<", "<<velocity.y<<")";
            graphicsManager_->editor->AddInspectorInfo("PlayerVel", oss.str());
        }
#endif

    }

    void Destroy() override
    {
        physicsManager.Destroy();
        MainEngine::Destroy();
    }

    void OnBeginContact(const neko::Collider* colliderA, const neko::Collider* colliderB) override
    {
        if(colliderA->entity == playerData.playerEntity && colliderA->fixture->IsSensor())
        {
            playerData.contactNmb++;
        }
        if (colliderB->entity == playerData.playerEntity && colliderB->fixture->IsSensor())
        {
            playerData.contactNmb++;
        }
    }
    void OnEndContact(const neko::Collider* colliderA, const neko::Collider* colliderB) override
    {
        if (colliderA->entity == playerData.playerEntity && colliderA->fixture->IsSensor())
        {
            playerData.contactNmb--;
        }
        if (colliderB->entity == playerData.playerEntity && colliderB->fixture->IsSensor())
        {
            playerData.contactNmb--;
        }
    }

protected:
    neko::Timer physicsTimer{0.0f,0.0f};
    neko::EntityManager entityManager;
    multi::SpriteManager spriteManager;
    neko::TextureManager textureManager;
    neko::Position2dManager positionManager;
    neko::Scale2dManager transformManager;
    neko::Angle2dManager angleManager;
    neko::Physics2dManager physicsManager;
    multi::ShapeManager shapeManager;

    PlayerData playerData = {};
    static const size_t platformsNmb = 3;
    const float jumpVelocity = 4.0f;
    const float moveVelocity = 2.0f;
};

TEST(Multi, PlatformerBasic)
{
    PlatformerEngine engine;
    engine.Init();
    engine.EngineLoop();
}
}