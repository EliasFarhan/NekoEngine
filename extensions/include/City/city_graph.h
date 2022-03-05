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
#include <queue>
#include <map>
#include "engine/globals.h"
#include "engine/system.h"
#include "engine/worker_system.h"

namespace neko
{
class TileMapGraph;

using NeighborBit = std::uint16_t;
enum class NeighborType : NeighborBit
{
    UP = 1 << 0,
    DOWN = 1 << 1,
    LEFT = 1 << 2,
    RIGHT = 1 << 3
};

const static std::map<NeighborType, sf::Vector2i> neighborTypeMap = {
        {NeighborType::UP, sf::Vector2i(0,-1)},
        {NeighborType::DOWN, sf::Vector2i(0,1)},
        {NeighborType::RIGHT, sf::Vector2i(1,0)},
        {NeighborType::LEFT, sf::Vector2i(-1,0)},
};
NeighborType GetNeighborType(const sf::Vector2i& direction);
sf::Vector2i GetDirection(NeighborType neighborType);

void AddNeighborToBit(NeighborBit& neighborBit, NeighborType neighborType);

void RemoveNeighborToBit(NeighborBit& neighborBit, NeighborType neighborType);

bool HasBitNeighbor(const NeighborBit& neighborBit, NeighborType neighborType);

constexpr static Index maxNeighborsNmb = 4;
struct Node
{
	sf::Vector2i position = INVALID_TILE_POS;
	std::array<Index, maxNeighborsNmb> neighborsIndex;
    NeighborBit neighborBitwise = 0u;
};

class PathFindingManager : public System
{
public:
    using PathId = std::uint64_t;
    
    PathFindingManager(TileMapGraph& graph);
    void Init() override;
    void Update(float dt) override;
    void Destroy() override;

    PathId SchedulePathFinding(const sf::Vector2i& startPos, const sf::Vector2i& endPos);
    bool IsPathDone(PathId id) const;
    std::vector<sf::Vector2i> GetPath(PathId id);
private:
    struct PathJob
    {
        PathId id;
        sf::Vector2i startPos;
        sf::Vector2i endPos;
    };
    mutable std::shared_mutex pathMutex_;
    mutable std::shared_mutex jobMutex_;
    std::condition_variable_any cond_;
    std::shared_ptr<Task> updateTask_;
    TileMapGraph& graph_;
    std::unordered_map<PathId, std::vector<sf::Vector2i>> pathMap_;
    std::queue<PathJob> jobQueue_;
    inline static PathId id_ = 1;
    bool isRunning_ = false;
};

class TileMapGraph
{
public:
    void AddNode(sf::Vector2i pos);
    void RemoveNode(sf::Vector2i pos);
    bool ContainNode(sf::Vector2i pos) const;
    const std::vector<Node>& GetNodesVector() const;

    [[nodiscard]] std::vector<sf::Vector2i> CalculateShortestPath(const sf::Vector2i& startPos, const sf::Vector2i& endPos) const;
	Node* GetClosestNode(sf::Vector2i position);
private:
    static void AddNeighbor(Node& node, NeighborType neighborBit);
    static void RemoveNeighbor(Node& node, NeighborType neighborBit);
    static bool HasNeighbor(const Node& node, NeighborType neighborBit)
    {
        return (node.neighborBitwise & static_cast<NeighborBit>(neighborBit)) == static_cast<NeighborBit>(neighborBit);
        
    }
    std::vector<Node> nodes_;
};
}
