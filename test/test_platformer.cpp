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
#include <graphics/shape.h>
#include "engine/engine.h"
#include "graphics/sprite.h"
#include "graphics/texture.h"
#include "engine/transform.h"
#include "physics/physics.h"
#include "engine/entity.h"
#include "utilities/time_utility.h"
#include "engine/input.h"
#include <engine/log.h>
#include <SFML/Window/Event.hpp>

struct PlayerData
{
	neko::Entity playerEntity;
	int contactNmb;
	b2Body* playerBody = nullptr;
};

class PlatformerEngine : public neko::MainEngine
{
public:
	void Init() override
	{
		config.bgColor = sf::Color::White;
		MainEngine::Init();
		physicsManager.Init();
		{
			playerData.playerEntity = 1;
			const auto playerPos = sf::Vector2f(200, 200);
			
			transformManager.positions.push_back(playerPos);
			transformManager.scales.emplace_back(1,1);
			transformManager.angles.push_back(0.0f);

			const Index textureIndex = textureManager.LoadTexture("data/sprites/hero/jump/hero1.png");
			std::shared_ptr<sf::Texture> texture = textureManager.GetTexture(textureIndex);
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
			physicsManager.colliders.push_back(mainPlayerCollider);

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
			physicsManager.colliders.push_back(footCollider);

			playerData.playerBody = physicsManager.CreateBody(bodyDef, fixtureDef, 2);
		}
		{
			const sf::Vector2f platformPositions[PlatformerEngine::platformsNmb] = {
				sf::Vector2f(200,400),
				sf::Vector2f(300,300),
				sf::Vector2f(300,500)
			};
			const Index textureIndex = textureManager.LoadTexture("data/sprites/platform.jpg");
			std::shared_ptr<sf::Texture> platformTexture = textureManager.GetTexture(textureIndex);

			for (auto i = 0u ; i < platformsNmb;i++)
			{
				transformManager.positions.push_back(platformPositions[i]);
				transformManager.scales.emplace_back(1, 1);
				transformManager.angles.push_back(0.0f);

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
				physicsManager.colliders.push_back(platformCollider);
				physicsManager.CreateBody(bodyDef, &fixtureDef, 1);
				spriteManager.AddSprite(platformTexture);
			}
		}
		physicsTimer.period = config.physicsTimeStep;
		physicsTimer.Reset();

	}


	

	void Update() override
	{

		MainEngine::Update();
		physicsTimer.Update(dt.asSeconds());
		if(physicsTimer.IsOver())
		{
			physicsManager.Update();
			physicsTimer.time += physicsTimer.period;
		}
		//Player management
		{
			//Jumping
			if(playerData.contactNmb > 0 && keyboardManager.IsKeyDown(sf::Keyboard::Space))
			{
				const auto playerVelocity = playerData.playerBody->GetLinearVelocity();
				playerData.playerBody->SetLinearVelocity(b2Vec2(playerVelocity.x, -jumpVelocity));
			}
			//Move horizontal
			{
				int move = 0;
				move -= keyboardManager.IsKeyHeld(sf::Keyboard::Left);
				move += keyboardManager.IsKeyHeld(sf::Keyboard::Right);
                {
                    std::ostringstream oss;
                    oss << "Player Key Move: "<<move;
                    logDebug(oss.str());
                }
				const auto playerVelocity = playerData.playerBody->GetLinearVelocity();
 				playerData.playerBody->SetLinearVelocity(b2Vec2(move*moveVelocity, playerVelocity.y));
			}

			transformManager.CopyPositionsFromPhysics2d(physicsManager, 0, 1);
			spriteManager.CopyTransformPosition(transformManager, 0, 1);
			spriteManager.PushCommands(graphicsManager, 0, 1);
		}


		{
			spriteManager.CopyTransformPosition(transformManager, 1, platformsNmb);
			spriteManager.PushCommands(graphicsManager, 1, platformsNmb);
		}
#ifdef __neko_dbg__
        {

            //Main playerbox
            shapeManager.CopyPosition(&transformManager.positions[0], 0, 1);
            shapeManager.PushCommands(graphicsManager, 0, 1+platformsNmb);
        }
        {
            std::ostringstream oss;
            oss << "("<<transformManager.positions[0].x<<", "<<transformManager.positions[0].y<<")";
            graphicsManager->editor.AddInspectorInfo("PlayerPos", oss.str());
        }
        {
            std::ostringstream oss;
            b2Vec2 velocity = physicsManager.bodies[0]->GetLinearVelocity();
            oss << "("<<velocity.x<<", "<<velocity.y<<")";
            graphicsManager->editor.AddInspectorInfo("PlayerVel", oss.str());
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
	neko::SpriteManager spriteManager;
	neko::TextureManager textureManager;
	neko::Transform2dManager transformManager;
	neko::Physics2dManager physicsManager;
    neko::ShapeManager shapeManager;
	PlayerData playerData = {};
	static const size_t platformsNmb = 3;
	const float jumpVelocity = 4.0f;
	const float moveVelocity = 2.0f;
};

TEST(Platformer, PlatformerBasic)
{
	PlatformerEngine engine;
	engine.Init();
	engine.EngineLoop();
}