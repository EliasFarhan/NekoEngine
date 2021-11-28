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

#include <City/city_graph.h>
#include <unordered_map>
#include <cmath>
#include <engine/log.h>

#include "allocator/allocator.h"
#include "City/city_engine.h"
#include "engine/engine.h"

#ifdef TRACY_ENABLE
#include <Tracy.hpp>
#endif
namespace std
{
template<>
struct hash<sf::Vector2i>
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

#ifdef TRACY_ENABLE
	ZoneScoped
#endif
	if (ContainNode(pos))
		return;
	Node newNode{};
	newNode.position = pos;

	nodes_.push_back(newNode);
	auto* nodePtr = &nodes_.back();
	for (int dx = -1; dx <= 1; dx++)
	{
		for (int dy = -1; dy <= 1; dy++)
		{
			if (dx == dy || dx == -dy)
				continue;
			const sf::Vector2i dir = sf::Vector2i(dx, dy);
			const sf::Vector2i neighborPos = pos + dir;
			auto neighborIt = std::ranges::find_if(nodes_, [&neighborPos](const Node& node)
            {
                return node.position == neighborPos;
            });
			if (neighborIt != nodes_.end())
			{
				{
					const auto neighborType = GetNeighborType(-dir);
					const auto neighborIndex = static_cast<Index>(std::log2(static_cast<double>(neighborType)));
					neighborIt->neighborsIndex[neighborIndex] = static_cast<Index>(nodes_.size() - 1);
					AddNeighbor(*neighborIt, neighborType);
				}
				{
					const auto neighborType = GetNeighborType(dir);
					const auto neighborIndex = static_cast<Index>(std::log2(static_cast<double>(neighborType)));
					nodePtr->neighborsIndex[neighborIndex] = static_cast<Index>(neighborIt - nodes_.begin());
					AddNeighbor(*nodePtr, neighborType);
				}
			}
		}
	}
}

void TileMapGraph::RemoveNode(sf::Vector2i pos)
{

#ifdef TRACY_ENABLE
	ZoneScoped
#endif
	const auto nodeIt = std::ranges::find_if(nodes_, [&pos](const Node& node)
    {
        return node.position == pos;
    });
	//Cannot remove a node that does not exist
	if (nodeIt == nodes_.end())
		return;
	const Index nodeIndex = static_cast<Index>(nodeIt - nodes_.begin());
	nodes_.erase(nodeIt);
	for (auto& otherNode : nodes_)
	{
		for (Index i = 0; i < maxNeighborsNmb; i++)
		{
			if (otherNode.position + GetDirection(static_cast<NeighborType>(1u << i)) == pos)
			{
				RemoveNeighbor(otherNode, static_cast<NeighborType>(1u << i));
				otherNode.neighborsIndex[i] = 0;
			}
			
			if (otherNode.neighborsIndex[i] > nodeIndex)
			{
				otherNode.neighborsIndex[i]--;
			}
			
		}
	}
}

float distance(const Node& nodeA, const Node& nodeB)
{
	const sf::Vector2f posA = sf::Vector2f(nodeA.position);
	const sf::Vector2f posB = sf::Vector2f(nodeB.position);
	const sf::Vector2f deltaPos = posB - posA;
	return std::sqrt(deltaPos.x * deltaPos.x + deltaPos.y * deltaPos.y);
}

float distance(const sf::Vector2i& posA, const sf::Vector2i& posB)
{
	const auto deltaPos = posB - posA;
	return std::sqrt(static_cast<float>(deltaPos.x * deltaPos.x + deltaPos.y * deltaPos.y));
}


std::vector<sf::Vector2i>
TileMapGraph::CalculateShortestPath(const sf::Vector2i& startPos, const sf::Vector2i& endPos) const
{
#ifdef TRACY_ENABLE
	ZoneScoped
#endif
	auto path = std::vector<sf::Vector2i>();
	if (startPos == INVALID_TILE_POS || endPos == INVALID_TILE_POS)
		return path;
	const auto startNodeIt = std::ranges::find_if(nodes_, [&startPos](const Node& node)
    {
        return node.position == startPos;
    });

	if (startNodeIt == nodes_.end())
	{
#ifdef __neko_dbg__
		logDebug("[Error] Because of start pos");
#endif
		return path;
	}
	const auto startNodeIndex = static_cast<Index>(std::distance(nodes_.begin(), startNodeIt));
	const auto endNodeIt = std::ranges::find_if(nodes_, [&endPos](const Node& node)
    {
        return node.position == endPos;
    });
	if (endNodeIt == nodes_.end())
	{
#ifdef __neko_dbg__
		logDebug("[Error] Because of end pos");
#endif
		return path;
	}
	const auto endNodeIndex = static_cast<Index>(std::distance(nodes_.begin(), endNodeIt));
	using NodePair = std::pair<Index, float>;
	struct NodePathData
	{
		float cost = -1.0f;
		Index parentIndex = INDEX_INVALID;
	};
	auto* engine = MainEngine::GetInstance();
	std::vector<NodePathData, StandardAllocator<NodePathData>> nodePathDatas(nodes_.size(), {engine->GetFrameAllocator()});
	auto comp = [](const NodePair& nodeA, const NodePair& nodeB) -> bool
	{
		return nodeA.second < nodeB.second;
	};
	std::vector<NodePair, StandardAllocator<NodePair>> frontier(StandardAllocator<NodePair>{engine->GetFrameAllocator()});
	frontier.reserve(nodes_.size());
	frontier.emplace_back(startNodeIndex, 0.0f);
	nodePathDatas[startNodeIndex].cost = 0.0f;
	nodePathDatas[startNodeIndex].parentIndex = startNodeIndex;

	while (!frontier.empty())
	{
		const auto currentNodePair = frontier.front();
		frontier.erase(frontier.begin());
		const auto currentNodeIndex = currentNodePair.first;
		auto& currentNode = nodes_[currentNodeIndex];
		if (currentNode.position == endPos)
		{
#ifdef __neko_dbg__
			logDebug("Manage to go to the end");
#endif
			break;
		}
		for (size_t i = 0; i < maxNeighborsNmb; i++)
		{
			if (!HasNeighbor(currentNode, static_cast<NeighborType>(1u << i))) continue;
			const auto neighborIndex = currentNode.neighborsIndex[i];
			const auto neighborPos = currentNode.position + GetDirection(static_cast<NeighborType>(1 << i));
			const float newCost = nodePathDatas[currentNodeIndex].cost + distance(neighborPos, currentNode.position);
			const auto neighborCost = nodePathDatas[neighborIndex].cost;
			if (neighborCost < 0.0f || newCost < neighborCost)
			{
				auto neighborIt = std::ranges::find_if(frontier, [&neighborIndex](const NodePair& pair)
                {
                    return pair.first == neighborIndex;
                });
				if (neighborCost < 0.0f && neighborIt != frontier.end())
				{

					//Remove pair and put it back with new cost
					auto neighbor = *neighborIt;
					frontier.erase(neighborIt);
					neighbor.second = newCost + distance(neighborPos, endPos);
					const auto lower = std::ranges::lower_bound(frontier, neighbor, comp);
					frontier.insert(lower, neighbor);

				}
				else
				{
					NodePair neighbor = { neighborIndex, newCost + distance(neighborPos, endPos) };
					const auto lower = std::ranges::lower_bound(frontier, neighbor, comp);
					frontier.insert(lower, neighbor);
				}
				nodePathDatas[neighborIndex] = { newCost, currentNodeIndex };
			}
		}
	}

	if (nodePathDatas[endNodeIndex].parentIndex != INDEX_INVALID)
	{
		path.push_back(endPos);
		auto parentIndex = nodePathDatas[endNodeIndex].parentIndex;
		while (parentIndex != startNodeIndex)
		{
			path.push_back(nodes_[parentIndex].position);
			if(nodePathDatas[parentIndex].parentIndex != INDEX_INVALID)
			{
				parentIndex = nodePathDatas[parentIndex].parentIndex;
			}
			else
			{
				return {};
			}
		}
		path.push_back(startPos);
		std::ranges::reverse(path);
	}
	else
	{
#ifdef __neko_dbg__
		logDebug("[Error] No parent for end");
#endif
	}
#ifdef TRACY_ENABLE
	ZoneValue(path.size());
#endif
	return path;
}

Node* TileMapGraph::GetClosestNode(sf::Vector2i position)
{

#ifdef TRACY_ENABLE
	ZoneScoped
#endif
	Node* closestNode = nullptr;
	float closestDistance = -1.0f;
	for(auto& node: nodes_)
	{
		const float currentDistance = distance(node.position, position);
		if(closestNode == nullptr || currentDistance < closestDistance)
		{
			closestNode = &node;
			closestDistance = currentDistance;
		}
	}
	return closestNode;
}

bool TileMapGraph::ContainNode(sf::Vector2i pos) const
{

#ifdef TRACY_ENABLE
	ZoneScoped
#endif
	const auto nodeIt = std::ranges::find_if(nodes_, [&pos](const Node& node)
    {
        return node.position == pos;
    });
	return nodeIt != nodes_.end();
}

NeighborType GetNeighborType(const sf::Vector2i& direction)
{

#ifdef TRACY_ENABLE
	ZoneScoped
#endif
	const static auto reverseMap = []() -> std::unordered_map<sf::Vector2i, NeighborType>
	{
		std::unordered_map<sf::Vector2i, NeighborType> reverse;
		std::ranges::for_each(neighborTypeMap,
                              [&reverse](const std::pair<NeighborType, sf::Vector2i>& pair)
                              {
                                  reverse.insert({ pair.second, pair.first });
                              });
		return reverse;
	}();
	const auto typeIt = reverseMap.find(direction);
	if (typeIt == reverseMap.end())
	{
		return static_cast<NeighborType>(0u);
	}
	else
	{
		return typeIt->second;
	}
}

sf::Vector2i GetDirection(NeighborType neighborType)
{

#ifdef TRACY_ENABLE
	ZoneScoped
#endif
	const auto neighborIt = neighborTypeMap.find(neighborType);
	if (neighborIt == neighborTypeMap.end())
	{
		return sf::Vector2i();
	}
	else
	{
		return neighborIt->second;
	}
}

void AddNeighborToBit(NeighborBit& node, NeighborType neighborType)
{
	node = node | static_cast<NeighborBit>(neighborType);
}
void RemoveNeighborToBit(NeighborBit& node, NeighborType neighborType)
{
	node = node & ~static_cast<NeighborBit>(neighborType);
}
bool HasBitNeighbor(const NeighborBit& node, NeighborType neighborType)
{
	return (node & static_cast<NeighborBit>(neighborType)) == static_cast<NeighborBit>(neighborType);
}

void TileMapGraph::AddNeighbor(Node& node, NeighborType neighborBit)
{
	AddNeighborToBit(node.neighborBitwise, neighborBit);
}

void TileMapGraph::RemoveNeighbor(Node& node, NeighborType neighborBit)
{
	RemoveNeighborToBit(node.neighborBitwise, neighborBit);
}

bool TileMapGraph::HasNeighbor(const Node& node, NeighborType neighborBit)
{
	return HasBitNeighbor(node.neighborBitwise, neighborBit);
}


}

