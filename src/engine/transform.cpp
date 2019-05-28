#include <engine/transform.h>
#include "engine/globals.h"
#include <physics/physics.h>
#include <glm/glm.hpp>

namespace neko
{

Transform2dManager::Transform2dManager()
{
	positions.reserve(InitEntityNmb);
	scales.reserve(InitEntityNmb);
	angles.reserve(InitEntityNmb);
}

void Transform2dManager::CopyPositionsFromPhysics2d(Physics2dManager& physics2dManager, size_t start, size_t length)
{
	for(auto i = start; i < start+length;i++)
	{
		positions[i] = meter2pixel(physics2dManager.bodies[i]->GetPosition());
	}
}

void Transform2dManager::CopyAnglesFromPhysics2d(Physics2dManager& physics2dManager, size_t start, size_t length)
{
	for (auto i = start; i < start + length; i++)
	{
		angles[i] = meter2pixel(glm::radians(physics2dManager.bodies[i]->GetAngle()));
	}
}
}
