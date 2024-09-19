#include "AStar.h"

#include "AI/Pathfinding/Edge.h"
#include "AI/Pathfinding/INavigatable.h"
#include "AI/Pathfinding/Node.h"

#include "Structures/PriorityHeap.h"

namespace Pathfinding
{
	template<class INPUT_IT, class UNARY_PREDICATE, class ITEM>
	constexpr INPUT_IT Find(INPUT_IT first, INPUT_IT last, UNARY_PREDICATE predicate, ITEM item)
	{
		for (; first != last; ++first)
		{
			if (predicate(*first, item))
			{
				return first;
			}
		}

		return last;
	}

	bool AStar::FindPath(const vec2& start, const vec2& end, INavigatable* graph, Heuristic heuristic, vector<Node*>& calculated)
	{
		if(graph == nullptr)
		{
			return false;
		}

		Node* startNode = graph->ClosestNode(start);
		Node* endNode = graph->ClosestNode(end);

		return FindPath(startNode, endNode, heuristic, calculated);
	}

	bool AStar::FindPath(Node* start, Node* end, const Heuristic heuristic, vector<Node*>& calculated)
	{
		calculated.clear();

		if(start == end || start == nullptr || end == nullptr)
		{
			return false;
		}

		AStarNode* startNode = new AStarNode
		{
			0, 0, 0, start, nullptr
		};

		priority_heap<AStarNode*> open;
		list<AStarNode*> closed;

		open.push(startNode);

		bool foundPath = false;

		AStarNode* endNode = nullptr;

		auto comparator = [](const AStarNode* a, const Node* b)
			{
				return a->node == b;
			};

		while(!open.empty())
		{
			AStarNode* current = open.top();

			if(current->node == end)
			{
				endNode = current;

				foundPath = true;
				break;
			}

			open.pop();
			closed.emplace_back(current);

			for(const auto& edge : current->node->Edges())
			{
				if(Find(closed.begin(), closed.end(), comparator, edge->endpoint) == closed.end())
				{
					const float gScore = current->gScore + edge->cost;
					const float hScore = heuristic(edge->endpoint, end);
					const float fScore = gScore + hScore;

					const auto c = Find(open.begin(), open.end(), comparator, edge->endpoint);

					if(c == open.end())
					{
						AStarNode* newNode = new AStarNode
						{
							gScore, hScore, fScore, edge->endpoint, current
						};

						open.push(newNode);
						continue;
					}
					
					if(fScore < (*c)->fScore)
					{
						(*c)->gScore = gScore;
						(*c)->hScore = hScore;
						(*c)->fScore = fScore;
						(*c)->previous = current;
					}
				}
			}
		}

		if(foundPath)
		{
			AStarNode* current = endNode;

			while(current != nullptr)
			{
				calculated.emplace(calculated.begin(), current->node);

				current = current->previous;
			}
		}

		while(!open.empty())
		{
			delete open.top();
			open.pop();
		}

		for(const auto& node : closed)
		{
			delete node;
		}

		closed.clear();

		return foundPath;
	}
}
