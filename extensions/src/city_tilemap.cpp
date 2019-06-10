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

namespace neko
{
void CityBuilderTilemap::Init(TextureManager& textureManager)
{
	tilesheet.tilemap[0].setPrimitiveType(sf::Triangles);
	tilesheet.tilemap[1].setPrimitiveType(sf::Triangles);
	tilesheet.texture = textureManager.LoadTexture(textureName);
	tilesheet.texture->setRepeated(true);
	//GRASS
	{
		const auto grassIndex = int(CityBuilderTileType::GRASS);
		tilesheet.center[grassIndex] = sf::Vector2f(tileSize) / 2.0f;
		tilesheet.rect[grassIndex] = sf::FloatRect(0, 0, tileSize.x, tileSize.y);
		//Debug: just fill the vertex array with grass
		for(auto x = 0u; x < mapSize.x; x++)
		{
			for(auto y = 0u; y < mapSize.y; y++)
			{
				const auto position = sf::Vector2f(x * tileSize.x, y * tileSize.y);
				
				sf::Vertex quad[6];
				const auto rect = tilesheet.rect[grassIndex];
				const auto center = tilesheet.center[grassIndex];
				quad[0].position = position - center;
				quad[1].position = position - sf::Vector2f(center.x, -center.y);
				quad[2].position = position + center;
				quad[3].position = position - center;
				quad[4].position = position + center;
				quad[5].position = position - sf::Vector2f(-center.x, center.y);

				quad[0].texCoords = sf::Vector2f(
					rect.left, 
					rect.top);
				quad[1].texCoords = sf::Vector2f(
					rect.left,
					rect.top+rect.height);
				quad[2].texCoords = sf::Vector2f(
					rect.left+rect.width,
					rect.top+rect.height);
				quad[3].texCoords = sf::Vector2f(
					rect.left,
					rect.top);
				quad[4].texCoords = sf::Vector2f(
					rect.left+rect.width,
					rect.top+rect.height);
				quad[5].texCoords = sf::Vector2f(
					rect.left+rect.width,
					rect.top);
				for (auto& v : quad)
				{
					tilesheet.tilemap[0].append(v);
					tilesheet.tilemap[1].append(v);
				}
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
}
