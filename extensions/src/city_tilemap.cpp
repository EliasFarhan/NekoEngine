/*
 MIT License

 Copyright (c) 2019 SAE Institute Switzerland AG

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.
 */
#include <city_tilemap.h>
#include <graphics/texture.h>
#include <engine/engine.h>
#include <Remotery.h>
#include <city_map.h>

namespace neko
{
void CityBuilderTilemap::Init(TextureManager& textureManager)
{

	tilesheet.texture = textureManager.LoadTexture(textureName);
	tilesheet.texture->setRepeated(true);
	
	{
		//GRASS TILE
		const auto grassIndex = int(CityBuilderTileType::GRASS);
		tilesheet.center[grassIndex] = sf::Vector2f(tileSize) / 2.0f;
		tilesheet.rect[grassIndex] = sf::FloatRect(0, 0, tileSize.x, tileSize.y);

	}
	{
		//WATER
		auto waterIndex = int(CityBuilderTileType::WATER_VERTICAL);
		tilesheet.center[waterIndex] = sf::Vector2f(tileSize) / 2.0f;
		tilesheet.rect[waterIndex] = sf::FloatRect(
			7 * tileSize.x, 
			5 * tileSize.y, 
			tileSize.x, 
			tileSize.y);
		waterIndex = int(CityBuilderTileType::WATER_HORIZONTAL);
		tilesheet.center[waterIndex] = sf::Vector2f(tileSize) / 2.0f;
		tilesheet.rect[waterIndex] = sf::FloatRect(
			7 * tileSize.x,
			4 * tileSize.y,
			tileSize.x,
			tileSize.y);
		waterIndex = int(CityBuilderTileType::WATER_DOWN_LEFT);
		tilesheet.center[waterIndex] = sf::Vector2f(tileSize) / 2.0f;
		tilesheet.rect[waterIndex] = sf::FloatRect(
			6 * tileSize.x,
			4 * tileSize.y,
			tileSize.x,
			tileSize.y);
		waterIndex = int(CityBuilderTileType::WATER_DOWN_RIGHT);
		tilesheet.center[waterIndex] = sf::Vector2f(tileSize) / 2.0f;
		tilesheet.rect[waterIndex] = sf::FloatRect(
			5 * tileSize.x,
			4 * tileSize.y,
			tileSize.x,
			tileSize.y);
		waterIndex = int(CityBuilderTileType::WATER_UP_LEFT);
		tilesheet.center[waterIndex] = sf::Vector2f(tileSize) / 2.0f;
		tilesheet.rect[waterIndex] = sf::FloatRect(
			6 * tileSize.x,
			5 * tileSize.y,
			tileSize.x,
			tileSize.y);
		waterIndex = int(CityBuilderTileType::WATER_UP_RIGHT);
		tilesheet.center[waterIndex] = sf::Vector2f(tileSize) / 2.0f;
		tilesheet.rect[waterIndex] = sf::FloatRect(
			5 * tileSize.x,
			5 * tileSize.y,
			tileSize.x,
			tileSize.y);
	}
	{
		//TREES
		const auto treesIndex = int(CityBuilderTileType::TREES);
		tilesheet.center[treesIndex] = sf::Vector2f(tileSize.x, 2.0f * tileSize.y) / 2.0f;
		tilesheet.rect[treesIndex] = sf::FloatRect(5 * tileSize.x, tileSize.y, tileSize.x, 2 * tileSize.y);
	}
	tilesheet.tilemap[0].setPrimitiveType(sf::Triangles);
	tilesheet.tilemap[1].setPrimitiveType(sf::Triangles);
}

void CityBuilderTilemap::UpdateTilemap(CityBuilderMap& cityBuilderMap)
{
	tilesheet.tilemap[0].clear();
	tilesheet.tilemap[1].clear();
	//ENVIRONMENT
	{
		const int grassIndex = int(CityBuilderTileType::GRASS);
		//Fill the vertex array with grass
		for (auto x = 0u; x < cityBuilderMap.city.mapSize.x; x++)
		{
			for (auto y = 0u; y < cityBuilderMap.city.mapSize.y; y++)
			{

				const auto position = sf::Vector2f(x * tileSize.x, y * tileSize.y);
				auto rect = tilesheet.rect[ grassIndex ];
				auto center = tilesheet.center[grassIndex];
				AddNewTile(position, sf::Vector2f(tileSize), rect, center);
			}
		}
		//Add water on top of it
		for (auto x = 0u; x < cityBuilderMap.city.mapSize.x; x++)
		{
			for (auto y = 0u; y < cityBuilderMap.city.mapSize.y; y++)
			{
				
				int waterIndex = 0;
				bool direction[] = { 0,0,0,0 };//UP, DOWN, LEFT, RIGHT

				const sf::Vector2i tmpPos = sf::Vector2i(x, y);
				if (cityBuilderMap.environmentTiles[cityBuilderMap.Position2Index(tmpPos)] != EnvironmentTile::WATER)
				{
					continue;
				}
				//UP
				{
					const sf::Vector2i upPos = tmpPos + sf::Vector2i(0, -1);
					if(upPos.y < 0 ||
						cityBuilderMap.environmentTiles[cityBuilderMap.Position2Index(upPos)] == EnvironmentTile::WATER)
					{
						direction[0] = true;
					}
				}
				//DOWN
				{
					const sf::Vector2i downPos = tmpPos + sf::Vector2i(0, 1);
					if (downPos.y == cityBuilderMap.city.mapSize.y || 
						cityBuilderMap.environmentTiles[cityBuilderMap.Position2Index(downPos)] == EnvironmentTile::WATER)
					{
						direction[1] = true;
					}
				}
				//LEFT
				{
					const sf::Vector2i leftPos = tmpPos + sf::Vector2i(-1, 0);
					if (leftPos.x > 0 && 
						cityBuilderMap.environmentTiles[cityBuilderMap.Position2Index(leftPos)] == EnvironmentTile::WATER)
					{
						direction[2] = true;
					}
				}
				//RIGHT
				{
					const sf::Vector2i rightPos = tmpPos + sf::Vector2i(1,0);
					if (rightPos.x < cityBuilderMap.city.mapSize.x && 
						cityBuilderMap.environmentTiles[cityBuilderMap.Position2Index(rightPos)] == EnvironmentTile::WATER)
					{
						direction[3] = true;
					}
				}
				if (direction[1] && direction[3])
				{
					waterIndex = int(CityBuilderTileType::WATER_DOWN_RIGHT);
				}
				if (direction[1] && direction[2])
				{
					waterIndex = int(CityBuilderTileType::WATER_DOWN_LEFT);
				}
				if (direction[0] && direction[3])
				{
					waterIndex = int(CityBuilderTileType::WATER_UP_RIGHT);
				}
				if (direction[0] && direction[2])
				{
					waterIndex = int(CityBuilderTileType::WATER_UP_LEFT);
				}
				if(direction[0] && direction[1])
				{
					waterIndex = int(CityBuilderTileType::WATER_VERTICAL);
				}
				if(direction[2] && direction[3])
				{
					waterIndex = int(CityBuilderTileType::WATER_HORIZONTAL);
				}
				const auto position = sf::Vector2f(x * tileSize.x, y * tileSize.y);
				auto rect = tilesheet.rect[waterIndex];
				auto center = tilesheet.center[waterIndex];
				AddNewTile(position, sf::Vector2f(tileSize), rect, center);
			}
		}
	}
	{
		//TREES
		const int treesIndex = int(CityBuilderTileType::TREES);

		for (auto& element : cityBuilderMap.elements)
		{
			switch (element.elementType)
			{
			case CityElementType::TREES:
			{

				if(cityBuilderMap.environmentTiles[cityBuilderMap.Position2Index(element.position)] == EnvironmentTile::WATER)
				{
					continue;
				}
				const auto position = sf::Vector2f(
					element.position.x * tileSize.x,
					element.position.y * tileSize.y);
				const auto rect = tilesheet.rect[treesIndex];
				const auto center = tilesheet.center[treesIndex];
				const auto size = sf::Vector2f(
					element.size.x * tileSize.x,
					element.size.y * tileSize.y
				);

				AddNewTile(position, size, rect, center);
				break;
			}
			default:
				break;
			}
		}
	}
}

void CityBuilderTilemap::PushCommand(GraphicsManager* graphicsManager)
{
	rmt_ScopedCPUSample(PushCityTilemapCommands, 0);
	const int frameIndex = MainEngine::GetInstance()->frameIndex % 2;
	graphicsManager->Draw(&tilesheet.tilemap[frameIndex], tilesheet.texture);

}

void CityBuilderTilemap::AddNewTile(
	const sf::Vector2f position,
	const sf::Vector2f size,
	const sf::FloatRect rect, 
	const sf::Vector2f center)
{
	const sf::Vector2f sizeOffset = (size - sf::Vector2f(tileSize));
	sf::Vertex quad[6];
	quad[0].position = position - center - sizeOffset;
	quad[1].position = position - sf::Vector2f(center.x, -center.y) + sizeOffset;
	quad[2].position = position + center - sizeOffset;
	quad[3].position = position - center - sizeOffset;
	quad[4].position = position + center - sizeOffset;
	quad[5].position = position - sf::Vector2f(-center.x, center.y) - sizeOffset;

	quad[0].texCoords = sf::Vector2f(
		rect.left,
		rect.top);
	quad[1].texCoords = sf::Vector2f(
		rect.left,
		rect.top + rect.height);
	quad[2].texCoords = sf::Vector2f(
		rect.left + rect.width,
		rect.top + rect.height);
	quad[3].texCoords = sf::Vector2f(
		rect.left,
		rect.top);
	quad[4].texCoords = sf::Vector2f(
		rect.left + rect.width,
		rect.top + rect.height);
	quad[5].texCoords = sf::Vector2f(
		rect.left + rect.width,
		rect.top);
	for (auto& v : quad)
	{
		tilesheet.tilemap[0].append(v);
		tilesheet.tilemap[1].append(v);
	}
}
}
