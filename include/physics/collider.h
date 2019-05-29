#pragma once
#include "Box2D/Dynamics/b2Fixture.h"
#include "engine/entity.h"
#include "Box2D/Dynamics/b2WorldCallbacks.h"

namespace neko
{

class MainEngine;

struct Collider
{
	b2Fixture* fixture;
	b2Body* body;
	neko::Entity entity;
#ifdef __neko_dbg__
	int shapeIndex = -1;
#endif
};


class CollisionListener : public b2ContactListener
{
public:
	~CollisionListener() override;
	void BeginContact(b2Contact* contact) override;
	void EndContact(b2Contact* contact) override;
};
}
