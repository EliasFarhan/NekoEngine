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
const std::vector<Node>& TileMapGraph::GetNodesVector() const
{
    return nodes_;
}


void TileMapGraph::AddNode(sf::Vector2i pos)
{
    auto checkIfAlreadyExistResult = std::find_if(nodes_.begin(),nodes_.end(),[&pos](const Node& node){
        return node.position == pos;
    });
    if (checkIfAlreadyExistResult != nodes_.end())
        return;
    Node newNode{};
    newNode.position = pos;

    for (int dx = -1; dx <= 1; dx++)
    {
        for (int dy = -1; dy <= 1; dy++)
        {
            sf::Vector2i neighborPos = pos + sf::Vector2i(dx, dy);
            auto neighborIt = std::find_if(nodes_.begin(),nodes_.end(),[&neighborPos](const Node& node){
                return node.position == neighborPos;
            });
            if (neighborIt != nodes_.end())
            {
                neighborIt->neighbors[neighborIt->neighborsSize] = pos;
                neighborIt->neighborsSize++;

                newNode.neighbors[newNode.neighborsSize] = neighborIt->position;
                newNode.neighborsSize++;
            }
        }
    }
    nodes_.push_back(newNode);
}

void TileMapGraph::RemoveNode(sf::Vector2i pos)
{
    auto nodeIt = std::find_if(nodes_.begin(),nodes_.end(),[&pos](const Node& node){
        return node.position == pos;
    });
    //Cannot remove a node that does not exist
    if (nodeIt == nodes_.end())
        return;
    nodes_.erase(nodeIt);
    for(auto& otherNode : nodes_)
    {
        for(int i = 0; i < otherNode.neighborsSize;i++)
        {
            if(otherNode.neighbors[i] == pos)
            {
                for(int j = i+1; j < otherNode.neighborsSize;j++)
                {
                    otherNode.neighbors[j-1] = otherNode.neighbors[j];
                }
                otherNode.neighborsSize--;
                if(otherNode.neighborsSize < 0)
                {
                    otherNode.neighborsSize = 0;
                }
                break;
            }
        }
    }
}

const std::vector<sf::Vector2i>
TileMapGraph::CalculateShortestPath(const sf::Vector2i& startPos, const sf::Vector2i& endPos) const
{
    return std::vector<sf::Vector2i>();
}


}