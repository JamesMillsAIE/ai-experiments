#pragma once

#include <functional>
#include <list>

#include <Core/Debugging/IDebugHandler.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "PathfindingGraph.h"
#include "PathfindingNode.h"

using std::function;
using std::list;

using glm::vec2;
using glm::vec3;

using Debugging::IDebugHandler;
using Debugging::Debugger;
using Debugging::EVerbosity;

class Renderer2D;
class Texture;

namespace Pathfinding
{
	class AStarNode final : public PathfindingNode
	{
	public:
		float gScore;
		float hScore;

	public:
		explicit AStarNode(const vec2& pos);
		~AStarNode() override;

	public:
		float FScore() const;

	};

	typedef float(*Heuristic)(AStarNode*, AStarNode*, PathfindingGraph<AStarNode>*);

	class AStar
	{
	public:
		static bool FindPath(vec2 start, vec2 end, Heuristic heuristic, list<AStarNode*>& path);

		static void InitialiseFrom(float spacing, const string& texture, bool isObstacleMap = false);

	private:
		static PathfindingGraph<AStarNode>* m_graph;

	};
}