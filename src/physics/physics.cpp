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

#include <physics/physics.h>
#include <engine/engine.h>
#include "engine/globals.h"
#include "engine/log.h"
#include <sstream>

namespace neko
{

float Physics2dManager::pixelPerMeter = 100.0f;

void Physics2dManager::Init()
{
	auto* engine = BasicEngine::GetInstance();
	config_ = &engine->config;
	pixelPerMeter = config_->pixelPerMeter;
	world_ = std::make_unique<b2World>(config_->gravity);
	world_->SetContactListener(&collisionListener_);
	colliders_.reserve(INIT_ENTITY_NMB);
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
	for(auto i = 0u; i < fixturesNmb;i++)
	{
		auto* collider = &colliders_[colliderOffset+i];
		collider->body = body;
		fixturesDef[i].userData = collider;
		auto* fixture = body->CreateFixture(&fixturesDef[i]);
		collider->fixture = fixture;
	}
#ifdef __neko_dbg__
	{
		std::ostringstream oss;
		oss << "Body initialization: (" << body->GetPosition().x << ", " << body->GetPosition().y << "),\n("<<
			body->GetLinearVelocity().x<<", "<<body->GetLinearVelocity().y<<")";
		logDebug(oss.str());

	}
#endif
	return body;
}

void Physics2dManager::AddCollider(const Collider& collider)
{
    colliders_.push_back(collider);
}

b2Body* Physics2dManager::GetBodyAt(Index i)
{
    return bodies_[i];
}

float pixel2meter(float pixel)
{
	return pixel / Physics2dManager::pixelPerMeter;
}

b2Vec2 pixel2meter(const sf::Vector2f& pixel)
{
	return b2Vec2(pixel2meter(pixel.x), pixel2meter(pixel.y));
}


float meter2pixel(float meter)
{
	return meter * Physics2dManager::pixelPerMeter;
}

sf::Vector2f meter2pixel(const b2Vec2& meter)
{
	return sf::Vector2f(meter2pixel(meter.x), meter2pixel(meter.y));
}

Vec2f pixel2unit(const sf::Vector2f& v)
{
    return Vec2f(pixel2meter(v.x), pixel2meter(v.y));
}

float pixel2unit(const float v)
{
	return pixel2meter(v);
}

sf::Vector2f unit2pixel(const Vec2f& v)
{
    return sf::Vector2f(meter2pixel(v.x), meter2pixel(v.y));
}

float unit2pixel(float v)
{
	return meter2pixel(v);
}


void BodyDef2dManager::ParseComponentJson(json& componentJson, Entity entity)
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
    const auto& bodyDef = GetConstComponent(entity);
    json componentJson;
    componentJson["gravityScale"] = bodyDef.gravityScale;
    componentJson["type"] = bodyDef.type;
    componentJson["angularDamping"] = bodyDef.angularDamping;
    componentJson["linearDamping"] = bodyDef.linearDamping;
    componentJson["fixedRotation"] = bodyDef.fixedRotation;
    componentJson["allowSleep"] = bodyDef.allowSleep;
    return ComponentManager::SerializeComponentJson(entity);
}
}
