#pragma once

/*
 MIT License

 Copyright (c) 2017 SAE Institute Switzerland AG

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

#include <vector>
#include <memory>
#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <physics/physics.h>
#include <graphics/texture.h>

namespace neko
{
class GraphicsManager;
/**
 * \brief simple abstraction for one tilesheet with a tilemap texture with a double buffered vertex array
 */
struct Tilesheet
{
	std::shared_ptr<sf::Texture> texture = nullptr;
	/**
	 * \brief vertex array with 2 vectors for double buffering with the render thread
	 */
	sf::VertexArray tilemap[2];
};

class Tilemap
{
public:
    virtual void PushCommand(GraphicsManager* graphicsManager) = 0;
};

/**
 * \brief Abstraction of Tiled (https://www.mapeditor.org/) tilesheets
 */
struct Tiledsheet : public Tilesheet
{
	sf::Vector2u tileSize;
	sf::Vector2u size;
	unsigned firstId;
	unsigned tileNmb;
};

/*
* Abstraction of Tiled tilemap with different tilesheets with their own vertex arrays
*/
class TiledMap : public Tilemap
{
public:
    void
    Init(const std::string& tilemapPath, TextureManager& textureManager, Physics2dManager* physics2DManager = nullptr);
	void PushCommand(GraphicsManager* graphicsManager) override;
protected:
	std::vector<Tiledsheet> tileSheets_;

};
}