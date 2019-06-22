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
#include <unordered_map>
#include <queue>
#include <sstream>
#include <engine/log.h>
#include <Remotery.h>

namespace std
{
template<> struct hash<sf::Vector2i>
{
	typedef sf::Vector2i argument_type;
	typedef std::size_t result_type;
	result_type operator()(argument_type const& s) const noexcept
	{
		result_type const h1(std::hash<int>{}(s.x));
		result_type const h2(std::hash<int>{}(s.y));
		return h1 ^ (h2 << 1); // or use boost::hash_combine (see Discussion)
	}
};
}
namespace neko
{
const std::vector<Node>& TileMapGraph::GetNodesVector() const
{
	return nodes_;
}


void TileMapGraph::AddNode(sf::Vector2i pos)
{
	auto checkIfAlreadyExistResult = std::find_if(nodes_.begin(), nodes_.end(), [&pos](const Node& node) {
		return node.position == pos;
	});
	if (checkIfAlreadyExistResult != nodes_.end())
		return;
	Node newNode{};
	newNode.neighbors.fill(sf::Vector2i(-1, -1));
	newNode.position = pos;

	nodes_.push_back(newNode);
	for (int dx = -1; dx <= 1; dx++)
	{
		for (int dy = -1; dy <= 1; dy++)
		{
			if (dx == dy || dx == -dy)
				continue;
			sf::Vector2i neighborPos = pos + sf::Vector2i(dx, dy);
			auto neighborIt = std::find_if(nodes_.begin(), nodes_.end(), [&neighborPos](const Node& node) {
				return node.position == neighborPos;
			});
			if (neighborIt != nodes_.end())
			{
				auto checkNeighbor = std::find_if(neighborIt->neighbors.begin(), neighborIt->neighbors.begin() + neighborIt->neighborsSize, [&pos](sf::Vector2i localNeighborPos) {
					return pos == localNeighborPos;
				});
				if (checkNeighbor == neighborIt->neighbors.begin()+neighborIt->neighborsSize)
				{
					neighborIt->neighbors[neighborIt->neighborsSize] = pos;
					neighborIt->neighborsPtr[neighborIt->neighborsSize] = &nodes_.back();
					neighborIt->neighborsSize++;
				}

				newNode.neighbors[newNode.neighborsSize] = neighborIt->position;
				newNode.neighborsPtr[newNode.neighborsSize] = &(*neighborIt);
				newNode.neighborsSize++;
			}
		}
	}
}

void TileMapGraph::RemoveNode(sf::Vector2i pos)
{
	auto nodeIt = std::find_if(nodes_.begin(), nodes_.end(), [&pos](const Node& node) {
		return node.position == pos;
	});
	//Cannot remove a node that does not exist
	if (nodeIt == nodes_.end())
		return;
	nodes_.erase(nodeIt);
	for (auto& otherNode : nodes_)
	{
		for (int i = 0; i < otherNode.neighborsSize; i++)
		{
			if (otherNode.neighbors[i] == pos)
			{
				for (int j = i + 1; j < otherNode.neighborsSize; j++)
				{
					otherNode.neighbors[j - 1] = otherNode.neighbors[j];
					otherNode.neighborsPtr[j - 1] = otherNode.neighborsPtr[j];
				}
				otherNode.neighborsSize--;
				if (otherNode.neighborsSize < 0)
				{
					otherNode.neighborsSize = 0;
				}
				break;
			}
		}
	}
}

float distance(Node nodeA, Node nodeB)
{
	const sf::Vector2f posA = sf::Vector2f(nodeA.position);
	const sf::Vector2f posB = sf::Vector2f(nodeB.position);
	const sf::Vector2f deltaPos = posB - posA;
	return sqrtf(deltaPos.x*deltaPos.x + deltaPos.y*deltaPos.y);
}
float distance(sf::Vector2i posA, sf::Vector2i posB)
{
	const auto deltaPos = posB - posA;
	return sqrtf(deltaPos.x*deltaPos.x + deltaPos.y*deltaPos.y);
}





const std::vector<sf::Vector2i>
TileMapGraph::CalculateShortestPath(const sf::Vector2i& startPos, const sf::Vector2i& endPos) const
{
	rmt_ScopedCPUSample(GeneratePath, 0);
	auto path = std::vector<sf::Vector2i>();
	
	const auto startNodeIt = std::find_if(nodes_.begin(), nodes_.end(), [&startPos](const Node& node) {
		return node.position == startPos;
	});
	if (startNodeIt == nodes_.end())
	{
		logDebug("[Error] Because of start pos");
		return path;
	}
	const auto endNodeIt = std::find_if(nodes_.begin(), nodes_.end(), [&startPos](const Node& node) {
		return node.position == startPos;
	});
	if (endNodeIt == nodes_.end())
	{
		logDebug("[Error] Because of end pos");
		return path;
	}
	using NodePair = std::pair<sf::Vector2i, float>;
	std::unordered_map < sf::Vector2i, float> costs;
	std::unordered_map<sf::Vector2i, sf::Vector2i> parentMap;
	auto comp = [](const NodePair& nodeA, const NodePair& nodeB) -> bool
	{
		return nodeA.second < nodeB.second;
	};
	std::vector <NodePair> frontier;

	frontier.push_back(NodePair(startNodeIt->position, 0.0f));
	costs[startNodeIt->position] = 0.0f;
	parentMap[startNodeIt->position] = startNodeIt->position;

	while (!frontier.empty())
	{
		auto currentNodePair = frontier.front();
		frontier.erase(frontier.begin());
		auto currentPos = currentNodePair.first;
		auto currentNodeIt = std::find_if(nodes_.begin(), nodes_.end(), [&currentPos](const Node& node) {
			return node.position == currentPos;
		});
		if (currentNodeIt == nodes_.end())
		{
			std::ostringstream oss;
			oss << "No node for current node: (" << currentPos.x << ", " << currentPos.y << ")";
			logDebug(oss.str());
			continue;
		}
		if (currentPos == endPos)
		{
			logDebug("Manage to go to the end");
			break;
		}
		for (int i = 0; i < currentNodeIt->neighborsSize; i++)
		{
			const auto neighborPos = currentNodeIt->neighbors[i];
			const auto* neighborPtr = currentNodeIt->neighborsPtr[i];
			const float newCost = costs[currentNodeIt->position] + distance(neighborPos, endPos);
			auto costsIt = costs.find(neighborPos);
			if (costsIt == costs.end() || newCost < costs[neighborPos])
			{
				if (costsIt != costs.end())
				{
					auto neighborIt = std::find_if(frontier.begin(), frontier.end(), [&neighborPos](const NodePair& pair) {
						return pair.first == neighborPos;
					});
					auto neighbor = *neighborIt;
					frontier.erase(neighborIt);
					neighbor.second = newCost;
					auto upper = std::lower_bound(frontier.begin(), frontier.end(), neighbor, comp);
					frontier.insert(upper, neighbor);
					
				}
				else
				{
					NodePair neighbor;
					neighbor.first = neighborPos;
					neighbor.second = newCost;
					auto upper = std::lower_bound(frontier.begin(), frontier.end(), neighbor, comp);
					frontier.insert(upper, neighbor);
				}
				costs[neighborPos] = newCost;
				parentMap[neighborPos] = currentPos;
			}
		}
	}
	if (parentMap.find(endPos) != parentMap.end())
	{
		path.push_back(endPos);
		auto parentPos = parentMap[endPos];
		while (parentPos != startPos)
		{
			path.push_back(parentPos);
			parentPos = parentMap[parentPos];
		}
		path.push_back(startPos);
		std::reverse(path.begin(), path.end());
	}
	else
	{
		logDebug("[Error] No parent for end");
	}
	return path;
}


}

