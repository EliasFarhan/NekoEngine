#include <physics/physics.h>
#include <engine/engine.h>
#include "engine/globals.h"

namespace neko
{

float Physics2dManager::pixelPerMeter = 100.0f;

void Physics2dManager::Init()
{
	MainEngine* engine = MainEngine::GetInstance();
	config = &engine->config;
	pixelPerMeter = config->pixelPerMeter;
	world = new b2World(config->gravity);
	world->SetContactListener(&collisionListener);
	colliders.reserve(InitEntityNmb);
}

void Physics2dManager::Update()
{
	world->Step(config->physicsTimeStep, config->velocityIterations, config->positionIterations);
}

void Physics2dManager::Destroy()
{
	delete world;
}

b2Body* Physics2dManager::CreateBody(b2BodyDef& bodyDef, b2FixtureDef* fixturesDef, size_t fixturesNmb)
{
	auto* body = world->CreateBody(&bodyDef);
	bodies.push_back(body);
	const auto colliderOffset = colliders.size() - fixturesNmb;
	for(auto i = 0u; i < fixturesNmb;i++)
	{
		auto* collider = &colliders[colliderOffset+i];
		collider->body = body;
		fixturesDef[i].userData = collider;
		auto* fixture = body->CreateFixture(&fixturesDef[i]);
		collider->fixture = fixture;
	}
	return body;
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


}
