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

#include <city_graph.h>

namespace neko
{
const std::map<std::pair<float,float>, neko::Node>& Graph::GetNodesMap() const
{
    return nodesMap_;
}


void Graph::AddNode(sf::Vector2i pos)
{
    const auto posPair = std::pair<float,float>(pos.x, pos.y);
    if (!(nodesMap_.find(posPair) == nodesMap_.end()))
        return;
    Node newNode{};
    newNode.position = pos;

    for (int dx = -1; dx <= 1; dx++)
    {
        for (int dy = -1; dy <= 1; dy++)
        {
            sf::Vector2i neighborPos = pos + sf::Vector2i(dx, dy);
            const auto neighborPosPair = std::pair<float,float>(neighborPos.x, neighborPos.y);
            auto neighborIt = nodesMap_.find(neighborPosPair);
            if (neighborIt != nodesMap_.end())
            {
                neighborIt->second.neighbors[neighborIt->second.neighborsSize] = pos;
                neighborIt->second.neighborsSize++;

                newNode.neighbors[newNode.neighborsSize] = neighborIt->second.position;
                newNode.neighborsSize++;
            }
        }
    }
    nodesMap_[posPair] = newNode;
}

const std::vector<sf::Vector2i>
Graph::CalculateShortestPath(const sf::Vector2i& startPos, const sf::Vector2i& endPos) const
{
    return std::vector<sf::Vector2i>();
}

}