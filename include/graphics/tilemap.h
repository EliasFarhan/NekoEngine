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


#include <SFML/Graphics/VertexArray.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <physics/physics.h>
#include <graphics/texture.h>

namespace neko
{
class GraphicsManager;
struct TileSheet
{
    sf::Texture* texture = nullptr;
    sf::VertexArray tilemap[2]; //for double buffering
    sf::Vector2u tileSize;
    sf::Vector2u size;
    unsigned firstId;
    unsigned tileNmb;
};

class Tilemap
{
public:
    void PushCommand(GraphicsManager* graphicsManager);
protected:
    std::vector<TileSheet> tileSheets;
};


class TiledMap : public Tilemap
{
public:
    void
    Init(const std::string& tilemapPath, TextureManager& textureManager, Physics2dManager* physics2DManager = nullptr);

protected:


};
}