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
#include <sfml_engine/engine.h>
#include <sfml_engine/physics.h>
#include <engine/engine.h>
#include "engine/globals.h"
#include "engine/log.h"
#include <sstream>

namespace neko
{

float pixel2unit(float f)
{
	return f / box2d::Physics2dManager::pixelPerUnit;
}
Vec2f pixel2unit(sf::Vector2f v)
{
	return Vec2f(pixel2unit(v.x), pixel2unit(v.y));
}

float unit2pixel(float f)
{
	return f * box2d::Physics2dManager::pixelPerUnit;
}
sf::Vector2f unit2pixel(Vec2f v)
{
	return sf::Vector2f(unit2pixel(v.x), unit2pixel(v.y));
}

box2d::Physics2dManager::Physics2dManager(ContactListener& contactListener) :
	config_(nullptr),
	collisionListener_(contactListener)

{
}



namespace box2d
{
float box2d::Physics2dManager::pixelPerUnit = 100.0f;
float Physics2dManager::pixelPerMeter = 100.0f;

void Physics2dManager::Init()
{
	pixelPerMeter = config_->pixelPerMeter;
	world_ = std::make_unique<b2World>(b2Vec2(config_->gravityX, config_->gravityY));
	world_->SetContactListener(&collisionListener_);
	colliders_.reserve(INIT_ENTITY_NMB);
}

void Physics2dManager::SetConfiguration(Configuration* config)
{
	config_ = config;
}

void Physics2dManager::Update(float dt)
{
	(void)dt;//TODO put physics clock here
	world_->Step(config_->physicsTimeStep, config_->velocityIterations, config_->positionIterations);
}

void Physics2dManager::Destroy()
{
	world_ = nullptr;
}

b2Body* Physics2dManager::CreateBody(b2BodyDef& bodyDef, b2FixtureDef* fixturesDef, size_t fixturesNmb)
{
	auto* body = world_->CreateBody(&bodyDef);
	bodies_.push_back(body);
	const auto colliderOffset = colliders_.size() - fixturesNmb;
	for (auto i = 0u; i < fixturesNmb; i++)
	{
		auto* collider = &colliders_[colliderOffset + i];
		collider->body = body;
		fixturesDef[i].userData = collider;
		auto* fixture = body->CreateFixture(&fixturesDef[i]);
		collider->fixture = fixture;
	}
#ifdef __neko_dbg__
	{
		std::ostringstream oss;
		oss << "Body initialization: (" << body->GetPosition().x << ", " << body->GetPosition().y << "),\n(" <<
			body->GetLinearVelocity().x << ", " << body->GetLinearVelocity().y << ")";
		logDebug(oss.str());

	}
#endif
	return body;
}

void Physics2dManager::DestroyBody(b2Body* body) const
{
	world_->DestroyBody(body);
}

void Physics2dManager::AddCollider(const Collider& collider)
{
	colliders_.push_back(collider);
}

b2Body* Physics2dManager::GetBodyAt(Index i)
{
	return bodies_[i];
}

Body2dManager::Body2dManager(Physics2dManager& physics2dManager) :
	physics2dManager_(physics2dManager)
{
}

void Body2dManager::DestroyComponent(EntityManager& entityManager, Entity entity)
{
	ComponentManager<b2Body*, ComponentType(NekoComponentType::BODY2D)>::DestroyComponent(entityManager, entity);
	physics2dManager_.DestroyBody(components_[entity]);
}

void BodyDef2dManager::ParseComponentJson(const json& componentJson, Entity entity)
{
	b2BodyDef bodyDef;
	bodyDef.gravityScale = componentJson["gravityScale"];
	bodyDef.type = componentJson["type"];
	bodyDef.angularDamping = componentJson["angularDamping"];
	bodyDef.linearDamping = componentJson["linearDamping"];
	bodyDef.fixedRotation = componentJson["fixedRotation"];
	bodyDef.allowSleep = componentJson["allowSleep"];
	components_[entity] = bodyDef;
}

json BodyDef2dManager::SerializeComponentJson(Entity entity)
{
	const auto& bodyDef = GetComponent(entity);
	json componentJson;
	componentJson["gravityScale"] = bodyDef.gravityScale;
	componentJson["type"] = bodyDef.type;
	componentJson["angularDamping"] = bodyDef.angularDamping;
	componentJson["linearDamping"] = bodyDef.linearDamping;
	componentJson["fixedRotation"] = bodyDef.fixedRotation;
	componentJson["allowSleep"] = bodyDef.allowSleep;
	return ComponentManager::SerializeComponentJson(entity);
}
float pixel2meter(float f)
{
	return f / Physics2dManager::pixelPerMeter;
}

b2Vec2 pixel2meter(sf::Vector2f v)
{
	return b2Vec2(pixel2meter(v.x), pixel2meter(v.y));
}


float meter2pixel(float f)
{
	return f * Physics2dManager::pixelPerMeter;
}

sf::Vector2f meter2pixel(b2Vec2 v)
{
	return sf::Vector2f(meter2pixel(v.x), meter2pixel(v.y));
}
}
}