#include <physics/collider.h>
#include <Box2D/Box2D.h>
#include <engine/engine.h>

namespace neko
{
CollisionListener::~CollisionListener()
{
}

void CollisionListener::BeginContact(b2Contact* contact)
{
	auto* colliderA = static_cast<Collider*>(contact->GetFixtureA()->GetUserData());
	auto* colliderB = static_cast<Collider*>(contact->GetFixtureB()->GetUserData());
	MainEngine::GetInstance()->OnBeginContact(colliderA, colliderB);
}

void CollisionListener::EndContact(b2Contact* contact)
{
	auto* colliderA = static_cast<Collider*>(contact->GetFixtureA()->GetUserData());
	auto* colliderB = static_cast<Collider*>(contact->GetFixtureB()->GetUserData());
	MainEngine::GetInstance()->OnEndContact(colliderA, colliderB);
}
}
