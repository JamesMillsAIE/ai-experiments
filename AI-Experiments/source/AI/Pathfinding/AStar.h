#pragma once

#include <functional>
#include <list>

#include <Core/Debugging/IDebugHandler.h>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "IPathProvider.h"
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

	typedef function<float(PathfindingNode*, PathfindingNode*, PathfindingGraph<AStarNode>*)> Heuristic;

	class AStar final : public IPathProvider
	{
	public:
		AStar();
		~AStar() override;

	public:
		void InitialiseFrom(float spacing, const string& texture, bool isObstacleMap = false);
		void SetHeuristic(const Heuristic& heuristic);

		PathfindingNode* RandomNode(const Random* random) const override;
		PathfindingNode* GetClosestNode(const vec2& position) const override;
		bool FindPath(const vec2& start, const vec2& end, vector<vec2>& path) override;

	protected:
		string DebugCategory() override;
		void RenderDebuggingTools(Renderer2D* renderer, EVerbosity verbosity) override;
		void HandleImGui(EVerbosity verbosity) override;

	private:
		PathfindingGraph<AStarNode>* m_graph;
		Heuristic m_heuristic;

	};
}