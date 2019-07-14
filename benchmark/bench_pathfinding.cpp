#include <benchmark/benchmark.h>
#include <City/city_graph.h>
#include <unordered_map>
#include <cmath>

const unsigned long fromRange = 8;
const unsigned long toRange = 32768;

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

const static neko::Index maxNeighborsNmb = 4;
struct NewNode
{
	sf::Vector2i position = neko::INVALID_TILE_POS;
	std::array<neko::Index, maxNeighborsNmb> neighborsIndex;
};

float distance(const NewNode& nodeA, const NewNode& nodeB)
{
	const sf::Vector2f posA = sf::Vector2f(nodeA.position);
	const sf::Vector2f posB = sf::Vector2f(nodeB.position);
	const sf::Vector2f deltaPos = posB - posA;
	return sqrtf(deltaPos.x * deltaPos.x + deltaPos.y * deltaPos.y);
}

float distance(const sf::Vector2i& posA, const sf::Vector2i& posB)
{
	const auto deltaPos = posB - posA;
	return sqrtf(float(deltaPos.x * deltaPos.x + deltaPos.y * deltaPos.y));
}

std::unordered_map<sf::Vector2i, neko::Index> dirToIndex = {
	{sf::Vector2i(0,-1), 0},
	{sf::Vector2i(0,1), 1},
	{sf::Vector2i(-1,0), 2},
	{sf::Vector2i(1,0), 3},
};

class NewTileMapGraph
{
public:
	void AddNode(sf::Vector2i pos)
	{
		if (ContainNode(pos))
			return;
		NewNode newNode{};
		newNode.position = pos;
		newNode.neighborsIndex.fill(neko::INVALID_INDEX);
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
				auto neighborIt = std::find_if(nodes_.begin(), nodes_.end(), [&neighborPos](const NewNode& node)
				{
					return node.position == neighborPos;
				});
				if (neighborIt != nodes_.end())
				{
					{
						const neko::Index neighborIndex = dirToIndex[-dir];
						neighborIt->neighborsIndex[neighborIndex] = neko::Index(nodes_.size() - 1);

					}
					{
						const neko::Index neighborIndex = dirToIndex[dir];
						nodePtr->neighborsIndex[neighborIndex] = neko::Index(neighborIt - nodes_.begin());

					}
				}
			}
		}
	}
	void RemoveNode(sf::Vector2i pos)
	{
		const auto nodeIt = std::find_if(nodes_.begin(), nodes_.end(), [&pos](const NewNode& node)
		{
			return node.position == pos;
		});
		//Cannot remove a node that does not exist
		if (nodeIt == nodes_.end())
			return;
		const neko::Index nodeIndex = neko::Index(nodeIt - nodes_.begin());
		nodes_.erase(nodeIt);
		for (auto& otherNode : nodes_)
		{
			for (neko::Index i = 0; i < maxNeighborsNmb; i++)
			{
				if (otherNode.position + city::GetDirection(city::NeighborType(1u << i)) == pos)
				{
					otherNode.neighborsIndex[i] = neko::INVALID_INDEX;
				}

				if (otherNode.neighborsIndex[i] > nodeIndex)
				{
					otherNode.neighborsIndex[i]--;
				}

			}
		}
	}
	bool ContainNode(sf::Vector2i pos) const
	{
		const auto nodeIt = std::find_if(nodes_.begin(), nodes_.end(), [&pos](const NewNode& node)
		{
			return node.position == pos;
		});
		return nodeIt != nodes_.end();
	}
	const std::vector<NewNode>& GetNodesVector() const
	{
		return nodes_;
	}
	const std::vector<sf::Vector2i> CalculateShortestPath(const sf::Vector2i& startPos, const sf::Vector2i& endPos) const
	{
		auto path = std::vector<sf::Vector2i>();
		if (startPos == neko::INVALID_TILE_POS || endPos == neko::INVALID_TILE_POS)
			return path;
		const auto startNodeIt = std::find_if(nodes_.begin(), nodes_.end(), [&startPos](const NewNode& node)
		{
			return node.position == startPos;
		});

		if (startNodeIt == nodes_.end())
		{
			return path;
		}
		const auto endNodeIt = std::find_if(nodes_.begin(), nodes_.end(), [&startPos](const NewNode& node)
		{
			return node.position == startPos;
		});
		if (endNodeIt == nodes_.end())
		{
			return path;
		}
		using NodePair = std::pair<const NewNode*, float>;
		std::unordered_map<sf::Vector2i, float> costs;
		costs.reserve(nodes_.size());
		std::unordered_map<sf::Vector2i, sf::Vector2i> parentMap;
		parentMap.reserve(nodes_.size());
		auto comp = [](const NodePair& nodeA, const NodePair& nodeB) -> bool
		{
			return nodeA.second < nodeB.second;
		};
		std::vector<NodePair> frontier;
		frontier.reserve(nodes_.size());
		frontier.emplace_back(&(*startNodeIt), 0.0f);
		costs[startNodeIt->position] = 0.0f;
		parentMap[startNodeIt->position] = startNodeIt->position;

		while (!frontier.empty())
		{
			const auto currentNodePair = frontier.front();
			frontier.erase(frontier.begin());
			auto* currentNode = currentNodePair.first;

			if (currentNode->position == endPos)
			{
				break;
			}
			for (size_t i = 0; i < maxNeighborsNmb; i++)
			{
				if (currentNode->neighborsIndex[i] == neko::INVALID_INDEX) continue;

				const auto neighborPos = currentNode->position + city::GetDirection(city::NeighborType(1 << i));
				const auto* neighborPtr = &nodes_[currentNode->neighborsIndex[i]];
				const float newCost = costs[currentNode->position] + distance(neighborPos, currentNode->position);
				auto costsIt = costs.find(neighborPos);
				if (costsIt == costs.end() || newCost < costs[neighborPos])
				{
					auto neighborIt = std::find_if(frontier.begin(), frontier.end(), [&neighborPos](const NodePair& pair)
					{
						return pair.first->position == neighborPos;
					});
					if (costsIt != costs.end() && neighborIt != frontier.end())
					{

						//Remove pair and put it back with new cost
						auto neighbor = *neighborIt;
						frontier.erase(neighborIt);
						neighbor.second = newCost + distance(neighborPos, endPos);
						const auto lower = std::lower_bound(frontier.begin(), frontier.end(), neighbor, comp);
						frontier.insert(lower, neighbor);

					}
					else
					{
						NodePair neighbor = { neighborPtr, newCost + distance(neighborPos, endPos) };
						const auto lower = std::lower_bound(frontier.begin(), frontier.end(), neighbor, comp);
						frontier.insert(lower, neighbor);
					}
					costs[neighborPos] = newCost;
					parentMap[neighborPos] = currentNode->position;
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
				if (parentMap.find(parentPos) != parentMap.end())
				{
					parentPos = parentMap[parentPos];
				}
				else
				{
					return std::vector<sf::Vector2i>();
				}
			}
			path.push_back(startPos);
			std::reverse(path.begin(), path.end());
		}
		else
		{
		}
		return path;
	}
	NewNode* GetClosestNode(sf::Vector2i position)
	{
		return nullptr;
	}
private:
	std::vector<NewNode> nodes_;
};


void FillGraph(city::TileMapGraph& graph, std::uint64_t length, std::uint64_t roadNmb)
{

	sf::Vector2i direction = sf::Vector2i(1, 0);
	sf::Vector2i currentPos = sf::Vector2i(0, 0);
	graph.AddNode(currentPos);
	const auto inverseVector2i = [](const sf::Vector2i dir, bool negative = false)->sf::Vector2i {
		return (negative ? -1 : 1) * sf::Vector2i(dir.y, dir.x);
	};
	for (neko::Index i = 0u; i < roadNmb; i++)
	{
		const int randValue = rand() % 20;
		if (randValue == 0)
		{
			direction = inverseVector2i(direction);
		}
		if (randValue == 1)
		{
			direction = inverseVector2i(direction, true);
		}

		sf::Vector2i newPos = currentPos + direction;
		if (newPos.x < 0 || newPos.x >= length)
		{
			direction.x = -direction.x;
			newPos = currentPos + direction;
		}

		if (newPos.y < 0 || newPos.y >= length)
		{
			direction.y = -direction.y;
			newPos = currentPos + direction;
		}
		currentPos = newPos;
		if (graph.ContainNode(newPos))
		{
			i--;
			continue;
		}
		graph.AddNode(newPos);
	}
}
void FillNewGraph(NewTileMapGraph& graph, std::uint64_t length, std::uint64_t roadNmb)
{

	sf::Vector2i direction = sf::Vector2i(1, 0);
	sf::Vector2i currentPos = sf::Vector2i(0, 0);
	graph.AddNode(currentPos);
	const auto inverseVector2i = [](const sf::Vector2i dir, bool negative = false)->sf::Vector2i {
		return (negative ? -1 : 1) * sf::Vector2i(dir.y, dir.x);
	};
	for (neko::Index i = 0u; i < roadNmb; i++)
	{
		const int randValue = rand() % 20;
		if (randValue == 0)
		{
			direction = inverseVector2i(direction);
		}
		if (randValue == 1)
		{
			direction = inverseVector2i(direction, true);
		}

		sf::Vector2i newPos = currentPos + direction;
		if (newPos.x < 0 || newPos.x >= length)
		{
			direction.x = -direction.x;
			newPos = currentPos + direction;
		}

		if (newPos.y < 0 || newPos.y >= length)
		{
			direction.y = -direction.y;
			newPos = currentPos + direction;
		}
		currentPos = newPos;
		if (graph.ContainNode(newPos))
		{
			i--;
			continue;
		}
		graph.AddNode(newPos);
	}
}


static void BM_CityPathfinding(benchmark::State& state)
{
	const unsigned long length = state.range(0);
	const auto roadNmb = length * 2;
	city::TileMapGraph graph;
	FillGraph(graph, length, roadNmb);

	for (auto _ : state)
	{
		sf::Vector2i endPos;
		state.PauseTiming();
		{
			const auto nodes = graph.GetNodesVector();
			endPos = nodes[rand() % nodes.size()].position;
		}
		state.ResumeTiming();
		benchmark::DoNotOptimize(graph.CalculateShortestPath(graph.GetNodesVector()[0].position, endPos));

	}
}
BENCHMARK(BM_CityPathfinding)->Range(fromRange, toRange);


static void BM_NewPathfinding(benchmark::State& state)
{
	const unsigned long length = state.range(0);
	const auto roadNmb = length * 2;
	NewTileMapGraph graph;
	FillNewGraph(graph, length, roadNmb);

	for (auto _ : state)
	{
		sf::Vector2i endPos;
		state.PauseTiming();
		{
			const auto nodes = graph.GetNodesVector();
			endPos = nodes[rand() % nodes.size()].position;
		}
		state.ResumeTiming();
		benchmark::DoNotOptimize(graph.CalculateShortestPath(graph.GetNodesVector()[0].position, endPos));

	}
}
BENCHMARK(BM_NewPathfinding)->Range(fromRange, toRange);

BENCHMARK_MAIN();