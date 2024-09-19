#pragma once

#include <vector>

#include <glm/vec2.hpp>

using std::vector;

using glm::vec2;

namespace Pathfinding
{
	class Node;

	class INavigatable;

	typedef float(*Heuristic)(Node*, Node*);

	class AStar
	{
	public:
		static bool FindPath(const vec2& start, const vec2& end, INavigatable* graph, Heuristic heuristic, vector<Node*>& calculated);

	private:
		static bool FindPath(Node* start, Node* end, Heuristic heuristic, vector<Node*>& calculated);

	private:
		struct AStarNode
		{
			friend class AStar;

		public:
			float gScore;
			float hScore;
			float fScore;

			Node* node;
			AStarNode* previous;

		};

	};
}