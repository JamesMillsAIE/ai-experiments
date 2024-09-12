#pragma once

#include <list>
#include <vector>

#include <glm/vec2.hpp>

#include <sweep/cdt.h>

#include <AI/Pathfinding/IPathProvider.h>
#include <AI/Pathfinding/PathfindingGraph.h>

#include "NavMeshNode.h"

using std::list;
using std::vector;

using glm::vec2;

using p2t::CDT;
using p2t::Point;

using Debugging::IDebugHandler;
using Debugging::EVerbosity;

class Random;

namespace Pathfinding
{
	class NavMeshObstacle;

	class NavMesh final : public IPathProvider
	{
	public:
		NavMesh(float width, float height);
		~NavMesh() override;

	public:
		bool AddObstacle(vec2 pos, vec2 size, float padding);

		void AddRandomObstacles(const Random* random, int num, vec2 size, float padding);
		void Build();

		PathfindingNode* RandomNode(const Random* random) const override;
		PathfindingNode* GetClosestNode(const vec2& position) const override;
		bool FindPath(const vec2& start, const vec2& end, vector<vec2>& path) override;

	protected:
		string DebugCategory() override;
		void RenderDebuggingTools(Renderer2D* renderer, EVerbosity verbosity) override;
		void HandleImGui(EVerbosity verbosity) override;

	private:
		vector<NavMeshNode*> m_nodes;
		list<NavMeshObstacle*> m_obstacles;

		CDT* m_cdt;
		vector<vector<Point*>> m_polygons;

		vec2 m_extents;

		PathfindingGraph<NavMeshNode>* m_graph;

	private:
		vector<vec2> SmoothPath(const vector<NavMeshNode*>& path, vec2 start, vec2 end);

	};
}