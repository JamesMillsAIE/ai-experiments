#pragma once

#include <list>
#include <vector>

#include <glm/vec2.hpp>

#include <sweep/cdt.h>

#include "AI/Pathfinding/INavigatable.h"

using std::list;
using std::vector;

using glm::vec2;

using p2t::CDT;
using p2t::Point;

using Debugging::IDebugHandler;
using Debugging::EVerbosity;

namespace Pathfinding
{
	class NavigationMeshNode;
	class NavigationObstacle;

	class NavigationMesh final : public INavigatable
	{
	public:
		NavigationMesh(float width, float height);
		~NavigationMesh() override;

	public:
		bool AddObstacle(NavigationObstacle* obstacle);

		void Build();

		Node* ClosestNode(vec2 worldPos) override;
		Node* RandomNode(Random* random) override;
		vector<Node*> SmoothPath(const vector<Node*>& path) override;

	protected:
		string DebugCategory() override;
		void RenderDebuggingTools(Renderer* renderer, EVerbosity verbosity) override;
		void HandleImGui(EVerbosity verbosity) override;

	private:
		vector<NavigationMeshNode*> m_nodes;
		list<NavigationObstacle*> m_obstacles;

		CDT* m_cdt;
		vector<vector<Point*>> m_polygons;

		vec2 m_extents;

	};
}
