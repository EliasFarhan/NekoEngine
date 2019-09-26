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
#include <random>
#include <gtest/gtest.h>
#include <engine/engine.h>
#include <graphics/sprite.h>

#include <SFML/Graphics/RenderTexture.hpp>
#include <graphics/shape.h>
#include <utilities/time_utility.h>
#include <engine/log.h>


struct PlayerData
{
    neko::Entity playerEntity;
    int contactNmb;
    b2Body* playerBody = nullptr;
};

class BasicPlatformerEngine : public neko::BasicEngine
{
public:


    void Init() override
    {
        config.gameWindowSize = sf::Vector2u(1920, 1080);
        config.bgColor = sf::Color::White;
        BasicEngine::Init();
        renderTexture_.create(config.gameWindowSize.x, config.gameWindowSize.y);
        physicsManager_.Init();

        /*
         * Background
        */
        const auto bgEntity = entityManager_.CreateEntity();
        positionManager.AddComponent(entityManager_, bgEntity);
        positionManager.SetComponent(bgEntity, sf::Vector2f());
        const neko::Index bgTextureIndex = textureManager_.LoadTexture("data/sprites/bg/bg.png");
        spriteManager_.AddComponent(entityManager_, bgEntity);

        auto& bgSprite = spriteManager_.GetComponent(bgEntity);
        spriteManager_.CopyTexture(bgTextureIndex, bgEntity, 1);
        spriteManager_.CopyLayer(-1, bgEntity, 1);

        /*
         * Player initialization
         */
        playerData.playerEntity = entityManager_.CreateEntity();
        const auto playerPos = sf::Vector2f(200, 200);

        positionManager.AddComponent(entityManager_, playerData.playerEntity);
        positionManager.SetComponent(playerData.playerEntity, playerPos);

        const neko::Index playerTextureIndex = textureManager_.LoadTexture("data/sprites/hero/jump/hero1.png");
        auto* playerTexture = textureManager_.GetTexture(playerTextureIndex);
        spriteManager_.AddComponent(entityManager_, playerData.playerEntity);
        spriteManager_.CopyTexture(playerTextureIndex, playerData.playerEntity, 1);
        spriteManager_.CopySpriteOrigin(sf::Vector2f(0.5f, 0.5f), playerData.playerEntity, 1);

        const auto physicsSize = neko::pixel2meter(sf::Vector2f(playerTexture->getSize()));
        b2BodyDef bodyDef;
        bodyDef.position = neko::pixel2meter(playerPos);
        bodyDef.linearVelocity = b2Vec2(0, 0);
        bodyDef.fixedRotation = true;
        bodyDef.type = b2_dynamicBody;

        b2PolygonShape playerBox[2];
        playerBox[0].SetAsBox(physicsSize.x / 2.0f, physicsSize.y / 2.0f);

        const float footOffset = 5.0f;
        const b2Vec2 footPoints[3] = {
                b2Vec2(0, physicsSize.y / 2.0f),
                b2Vec2(neko::pixel2meter(footOffset), physicsSize.y / 2.0f + neko::pixel2meter(footOffset)),
                b2Vec2(neko::pixel2meter(-footOffset), physicsSize.y / 2.0f + neko::pixel2meter(footOffset)),
        };
        playerBox[1].Set(footPoints, 3);

        b2FixtureDef fixtureDef[2];

        neko::Collider mainPlayerCollider{};
#ifdef __neko_dbg__
        neko::ShapeDef shapeDef;
        shapeDef.fillColor = sf::Color::Transparent;
        shapeDef.outlineColor = sf::Color::Green;
        shapeDef.outlineThickness = 1.0f;
        shapeManager_.AddComponent(entityManager_, playerData.playerEntity);
        shapeManager_.AddBox(playerData.playerEntity, playerPos,
                             neko::meter2pixel(b2Vec2(physicsSize.x / 2.0f, physicsSize.y / 2.0f)), shapeDef);

#endif
        fixtureDef[0].shape = &playerBox[0];
        fixtureDef[0].friction = 0.0f;
        mainPlayerCollider.entity = playerData.playerEntity;
        physicsManager_.AddCollider(mainPlayerCollider);

        neko::Collider footCollider{};
        footCollider.entity = playerData.playerEntity;
        fixtureDef[1].shape = &playerBox[1];
        fixtureDef[1].isSensor = true;
#ifdef __neko_dbg__
        //TODO put the foot in another entity
        sf::Vector2f points[3];
        for (int i = 0; i < 3; i++)
        {
            points[i] = neko::meter2pixel(footPoints[i]);
        }

        //auto footShapeIndex = shapeManager.AddPolygon(neko::meter2pixel(footPoints[0]), points, 3, shapeDef);
        //footCollider.shapeIndex = footShapeIndex;
#endif
        physicsManager_.AddCollider(footCollider);

        playerData.playerBody = physicsManager_.CreateBody(bodyDef, fixtureDef, 2);
        bodyManager_.AddComponent(entityManager_, playerData.playerEntity);
        bodyManager_.SetComponent(playerData.playerEntity, playerData.playerBody);
        /*
         * Platforms Initialization
         */
        sf::Vector2f platformPositions[platformsNmb];
        std::uniform_real_distribution<float> randomX(
                0.0f,
                float(config.gameWindowSize.x));
        std::uniform_real_distribution<float> randomY(
                0.0f,
                float(config.gameWindowSize.y));
        std::default_random_engine generator;
        const neko::Index platformTextureIndex =
                textureManager_.LoadTexture("data/sprites/platform.jpg");
        auto* platformTexture = textureManager_.GetTexture(platformTextureIndex);

        for (auto i = 0u; i < platformsNmb; i++)
        {
            const auto platformEntity = entityManager_.CreateEntity();
            platformPositions[i] = sf::Vector2f(randomX(generator), randomY(generator));

            positionManager.AddComponent(entityManager_, platformEntity);
            positionManager.SetComponent(platformEntity, platformPositions[i]);

            const auto platformPhysicsSize = neko::pixel2meter(sf::Vector2f(platformTexture->getSize()));
            b2BodyDef platformBodyDef;
            platformBodyDef.position = neko::pixel2meter(platformPositions[i]);
            platformBodyDef.fixedRotation = true;
            platformBodyDef.type = b2_staticBody;

            b2PolygonShape platformBox;
            platformBox.SetAsBox(platformPhysicsSize.x / 2.0f, platformPhysicsSize.y / 2.0f);

            b2FixtureDef platformFixtureDef;
            platformFixtureDef.shape = &platformBox;

            neko::Collider platformCollider{};
            platformCollider.entity = playerData.playerEntity + i + 1;
#ifdef __neko_dbg__
            neko::ShapeDef platformShapeDef;
            platformShapeDef.fillColor = sf::Color::Transparent;
            platformShapeDef.outlineColor = sf::Color::Green;
            platformShapeDef.outlineThickness = 1.0f;
            shapeManager_.AddComponent(entityManager_, platformEntity);
            shapeManager_.AddBox(platformEntity, platformPositions[i], neko::meter2pixel(
	                                 b2Vec2(platformPhysicsSize.x / 2.0f, platformPhysicsSize.y / 2.0f)), platformShapeDef);

#endif
            physicsManager_.AddCollider(platformCollider);
            auto* body = physicsManager_.CreateBody(platformBodyDef, &platformFixtureDef, 1);

            bodyManager_.AddComponent(entityManager_, platformEntity);
            bodyManager_.SetComponent(platformEntity, body);

            spriteManager_.AddComponent(entityManager_, platformEntity);
            spriteManager_.CopyTexture(platformTextureIndex, platformEntity, 1);
            spriteManager_.CopySpriteOrigin(sf::Vector2f(0.5f, 0.5f), platformEntity, 1);
        }


        physicsTimer.period = config.physicsTimeStep;
        physicsTimer.Reset();
    }


    void Update(float dt) override
    {
        BasicEngine::Update(dt);
        physicsTimer.Update(dt);
        if (physicsTimer.IsOver())
        {
            physicsManager_.Update(config.physicsTimeStep);
            physicsTimer.time += physicsTimer.period;
        }

        //Jumping
        if (playerData.contactNmb > 0 && keyboardManager_.IsKeyDown(sf::Keyboard::Space))
        {
            const auto playerVelocity = playerData.playerBody->GetLinearVelocity();
            playerData.playerBody->SetLinearVelocity(b2Vec2(playerVelocity.x, -jumpVelocity));
        }
        //Move horizontal
        {
            int move = 0;
            move -= keyboardManager_.IsKeyHeld(sf::Keyboard::Left);
            move += keyboardManager_.IsKeyHeld(sf::Keyboard::Right);
#ifdef __neko_dbg__
            {
                std::ostringstream oss;
                oss << "Player Key Move: " << move;
                logDebug(oss.str() + " Player pos: " + std::to_string(playerData.playerBody->GetPosition().x) + ", " +
                         std::to_string(playerData.playerBody->GetPosition().y));
            }
#endif
            const auto playerVelocity = playerData.playerBody->GetLinearVelocity();
            playerData.playerBody->SetLinearVelocity(b2Vec2(move * moveVelocity, playerVelocity.y));
        }
        positionManager.CopyAllPositionsFromBody2d(entityManager_, bodyManager_);

        renderTexture_.clear(config.bgColor);

        tmpEntities = entityManager_.FilterEntities(neko::EntityMask(neko::NekoComponentType::POSITION2D) |
                                                    neko::EntityMask(neko::NekoComponentType::SPRITE2D));

        spriteManager_.CopyAllTransformPositions(entityManager_, positionManager);
        spriteManager_.PushAllCommands(entityManager_, graphicsManager_);

        tmpEntities = entityManager_.FilterEntities(neko::EntityMask(neko::NekoComponentType::POSITION2D) |
                                                    neko::EntityMask(neko::NekoComponentType::CONVEX_SHAPE2D));
        for (auto entity : tmpEntities)
        {
            shapeManager_.CopyTransformPosition(positionManager, entity, 1);
            shapeManager_.PushCommands(&graphicsManager_, entity, 1);
        }

        //Render the game texture into the window
        graphicsManager_.Render(renderTexture_);
        renderTexture_.display();

        sf::Sprite sprite(renderTexture_.getTexture());
        sprite.setScale(sf::Vector2f(float(config.realWindowSize.x) / config.gameWindowSize.x,
                                     float(config.realWindowSize.y) / config.gameWindowSize.y));
        renderWindow->draw(sprite);


    }

    void Destroy() override
    {
        BasicEngine::Destroy();
    }

    void OnBeginContact(const neko::Collider* colliderA, const neko::Collider* colliderB) override
    {
        if (colliderA->entity == playerData.playerEntity && colliderA->fixture->IsSensor())
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

private:

    sf::RenderTexture renderTexture_;

    neko::EntityManager entityManager_;

    neko::Physics2dManager physicsManager_;
    neko::Body2dManager bodyManager_;

    neko::Position2dManager positionManager;

    neko::TextureManager textureManager_;
    neko::SpriteManager spriteManager_{this->textureManager_};
    neko::GraphicsManager graphicsManager_;
    neko::ConvexShapeManager shapeManager_;

    PlayerData playerData;

    std::vector<neko::Entity> tmpEntities;

    neko::Timer physicsTimer{0.0f, 0.0f};
    static const size_t platformsNmb = 100;
    const float jumpVelocity = 4.0f;
    const float moveVelocity = 2.0f;
};

TEST(Platformer, BasicPlatformer)
{
    BasicPlatformerEngine engine;
    engine.Init();
    engine.EngineLoop();
}