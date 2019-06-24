#pragma once


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

#include <SFML/System/Vector2.hpp>
#include <array>
#include <vector>
#include <map>
#include "engine/globals.h"

namespace neko
{

struct Node
{
	sf::Vector2i position = sf::Vector2i(0,0);
    std::array<sf::Vector2i, 4> neighbors;
    std::array<Index, 4> neighborsIndex;
    size_t neighborsSize = 0;
};

class TileMapGraph
{
public:
    void AddNode(sf::Vector2i pos);
    void RemoveNode(sf::Vector2i pos);
    bool ContainNode(sf::Vector2i pos) const;
    const std::vector<Node>& GetNodesVector() const;

    const std::vector<sf::Vector2i> CalculateShortestPath(const sf::Vector2i& startPos, const sf::Vector2i& endPos) const;
private:
    std::vector<Node> nodes_;
};
}
