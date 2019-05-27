#pragma once
#include <vector>
#include "engine/system.h"
#include <Box2D/Box2D.h>

namespace neko
{
class Configuration;
class Physics2dManager : public System
{
public:
	void Init() override;
	void Update() override;
	void Destroy() override;

	b2World* world = nullptr;
	std::vector<b2Body*> bodies;
	Configuration* config = nullptr;
	
};
}
