#pragma once
#include <vector>
#include "SFML/System/Vector2.hpp"
namespace neko
{
class Transform2dManager
{
public:
	Transform2dManager();
	std::vector<sf::Vector2f> positions;
	std::vector<sf::Vector2f> scales;
	std::vector<float> angles;
};
}