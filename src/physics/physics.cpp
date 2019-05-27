#include <physics/physics.h>
#include "engine/engine.h"

namespace neko
{
void Physics2dManager::Init()
{
	config = &MainEngine::GetInstance()->config;
	world = new b2World(config->gravity);
}

void Physics2dManager::Update()
{
	world->Step(config->physicsTimeStep, config->velocityIterations, config->positionIterations);
}

void Physics2dManager::Destroy()
{
	delete world;
}
}
