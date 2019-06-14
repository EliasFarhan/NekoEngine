#include <city_map.h>
#include <PerlinNoise.hpp>

namespace neko
{
void CityBuilderMap::Init()
{
	siv::PerlinNoise perlinNoise;
	
	for(int x = 0; x < city.mapSize.x; x++)
	{
		for(int y = 0; y < city.mapSize.y; y++)
		{
			const sf::Vector2i pos = sf::Vector2i(x, y);

			const auto pnoise = perlinNoise.noise0_1(
				float(pos.x) / city.mapSize.x*city.perlinFreq,
				float(pos.y) / city.mapSize.y*city.perlinFreq);
			if(pnoise < city.forestRatio)
			{
				CityElement element{};
				element.position = pos;
				element.size = sf::Vector2u(1, 1);
				element.elementType = CityElementType::TREES;
				elements.push_back(element);
			}
		}
	}
}
void CityBuilderMap::Update()
{
}
void CityBuilderMap::Destroy()
{
}
}