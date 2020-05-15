#include "mathematics/basic.h"

#include <random>

namespace neko
{
float RandomRange(float start, float end)
{
	static std::random_device rd;  //Will be used to obtain a seed for the random number engine
	static std::mt19937 gen(rd()); //Standard mersenne_twister_engine seeded with rd()
	const std::uniform_real_distribution<> dis(start, end);
	return dis(gen);
}
}
