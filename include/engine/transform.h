#pragma once
#include <vector>
#include "SFML/System/Vector2.hpp"
#include "globals.h"

namespace neko
{
class Physics2dManager;
class Transform2dManager
{
public:
	Transform2dManager();
	void CopyPositionsFromPhysics2d(Physics2dManager& physics2dManager, size_t start = 0, size_t length=InitEntityNmb);
	void CopyAnglesFromPhysics2d(Physics2dManager& physics2dManager, size_t start = 0, size_t length = InitEntityNmb);
	std::vector<sf::Vector2f> positions;
	std::vector<sf::Vector2f> scales;
	std::vector<float> angles;
};
}
