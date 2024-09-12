#include "AStar.h"

#include <iostream>

#include <Core/Rendering/Texture.h>
#include <Core/Structures/PriorityHeap.h>

#include <glm/geometric.hpp>

#include "PathfindingEdge.h"

namespace Pathfinding
{
	struct NodeComparator
	{
	public:
		bool operator()(const AStarNode* a, const AStarNode* b) const
		{
			return a->FScore() > b->FScore();
		}

	};

	AStarNode::AStarNode(const vec2& pos)
		: PathfindingNode(pos), gScore{ 0 }, hScore{ 0 }
	{
	}

	AStarNode::~AStarNode() = default;

	float AStarNode::FScore() const
	{
		return gScore + hScore;
	}

	AStar::AStar()
		: m_graph{ nullptr }, m_heuristic{ nullptr }
	{
	}

	AStar::~AStar()
	{
		if (m_graph)
		{
			delete m_graph;
			m_graph = nullptr;
		}
	}

	void AStar::InitialiseFrom(float spacing, const string& texture, bool isObstacleMap)
	{
		if (!m_graph)
		{
			Texture* nodeMap = new Texture(texture);

			uint width = nodeMap->GetWidth();
			uint height = nodeMap->GetHeight();

			m_graph = new PathfindingGraph<AStarNode>(width, height, spacing);
			m_graph->BuildFrom(nodeMap, isObstacleMap);

			delete nodeMap;
		}
	}

	void AStar::SetHeuristic(const Heuristic& heuristic)
	{
		m_heuristic = heuristic;
	}

	PathfindingNode* AStar::RandomNode(const Random* random) const
	{
		return nullptr;
	}

	PathfindingNode* AStar::GetClosestNode(const vec2& position) const
	{
		return m_graph->GetNearest(position);
	}

	bool AStar::FindPath(const vec2& start, const vec2& end, vector<vec2>& path)
	{
		if (!m_graph || !m_heuristic)
		{
			return false;
		}

		AStarNode* startNode = m_graph->GetNearest(start);
		AStarNode* endNode = m_graph->GetNearest(end);

		if (!startNode || !endNode)
		{
			return false;
		}

		priority_heap<AStarNode*, vector<AStarNode*>, NodeComparator> openList;
		list<AStarNode*> closedList;

		openList.push(startNode);

		bool found = false;
		while (!openList.empty())
		{
			AStarNode* current = openList.top();
			if (current == endNode)
			{
				found = true;
				break;
			}

			openList.pop();
			closedList.emplace_back(current);

			for (const auto& connection : current->Edges())
			{
				if (AStarNode* aStarNode = dynamic_cast<AStarNode*>(connection->endPoint))
				{
					if (std::ranges::find(closedList, connection->endPoint) == closedList.end())
					{
						const float gScore = current->gScore + connection->cost;
						const float hScore = m_heuristic(current, aStarNode, m_graph);
						const float fScore = gScore + hScore;

						if (openList.find(aStarNode) == openList.end())
						{
							aStarNode->gScore = gScore;
							aStarNode->hScore = hScore;
							aStarNode->previous = current;

							openList.push(aStarNode);
						}
						else if (fScore < aStarNode->FScore())
						{
							aStarNode->gScore = gScore;
							aStarNode->previous = current;
						}
					}
				}
			}
		}

		if (found)
		{
			path.clear();

			AStarNode* current = endNode;
			while (current)
			{
				path.emplace_back(current->position);

				current = dynamic_cast<AStarNode*>(current->previous);
			}

			std::ranges::reverse(path);
		}

		return found;
	}

	string AStar::DebugCategory()
	{
		return "AStar";
	}

	void AStar::RenderDebuggingTools(Renderer2D* renderer, EVerbosity verbosity)
	{
		if(m_graph)
		{
			m_graph->RenderDebuggingTools(renderer, verbosity);
		}
	}

	void AStar::HandleImGui(EVerbosity verbosity)
	{
		if (m_graph)
		{
			m_graph->HandleImGui(verbosity);
		}
	}
}
