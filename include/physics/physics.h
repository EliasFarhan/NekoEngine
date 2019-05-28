#pragma once
#include <vector>
#include "engine/system.h"
#include <Box2D/Box2D.h>
#include "SFML/System/Vector2.hpp"
#include <vector>
#include "collider.h"

namespace neko
{
class Configuration;
class Physics2dManager : public System
{
public:
	void Init() override;
	void Update() override;
	void Destroy() override;


	b2Body* CreateBody(b2BodyDef& bodyDef, b2FixtureDef* fixturesDef, size_t fixturesNmb);

	b2World* world = nullptr;
	std::vector<b2Body*> bodies;
	std::vector<Collider> colliders;
	Configuration* config = nullptr;
	CollisionListener collisionListener;
	static float pixelPerMeter;
};

float meter2pixel(float v);
float pixel2meter(float v);
sf::Vector2f meter2pixel(const b2Vec2& v);
b2Vec2 pixel2meter(const sf::Vector2f& v);

}
