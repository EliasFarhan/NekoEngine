#pragma once
#include <vector>
#include <map>
#include "SFML/Graphics/Texture.hpp"

namespace neko
{
struct Anim
{
	std::string name = "";
	int freq = 60;
	std::vector<int> imgIndex;
};

struct Animator
{
	std::vector<Anim> anims;
	Anim* currentAnim = nullptr;
};

class AnimManager
{
public:

	std::vector<sf::Texture*> textures;
	std::vector<Animator> animators;
};
}
