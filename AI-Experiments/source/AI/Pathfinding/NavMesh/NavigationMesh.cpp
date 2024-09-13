#include "NavigationMesh.h"

#include <imgui.h>

#include <glm/geometric.hpp>

#include <aie/bootstrap/Random.h>
#include <aie/bootstrap/Renderer.h>

#include "Global.h"
#include "NavigationMeshNode.h"
#include "NavigationObstacle.h"

#include "Physics/Shapes/Triangle.h"

using Debugging::Debugger;

namespace Pathfinding
{
	NavigationMesh::NavigationMesh(const float width, const float height)
		: m_extents{ width, height }
	{
		constexpr float padding = 1.f;

		m_polygons.emplace_back();
		m_polygons[0].emplace_back(new Point(-padding, padding));
		m_polygons[0].emplace_back(new Point(-padding, height + padding));
		m_polygons[0].emplace_back(new Point(width + padding, height + padding));
		m_polygons[0].emplace_back(new Point(width + padding, -padding));

		m_cdt = new CDT(m_polygons[0]);
	}

	NavigationMesh::~NavigationMesh()
	{
		for (const auto& node : m_nodes)
		{
			delete node;
		}
		m_nodes.clear();

		for (const auto& obstacle : m_obstacles)
		{
			delete obstacle;
		}
		m_obstacles.clear();
	}

	bool NavigationMesh::AddObstacle(NavigationObstacle* obstacle)
	{
		for(const auto& ob : m_obstacles)
		{
			if(ob->Intersects(obstacle))
			{
				return false;
			}
		}

		m_obstacles.emplace_back(obstacle);

		m_polygons.emplace_back();
		for(auto& v : obstacle->GetPaddedVertices())
		{
			m_polygons.back().emplace_back(new Point(v.x, v.y));
		}

		m_cdt->AddHole(m_polygons.back());

		return true;
	}

	void NavigationMesh::Build()
	{
		m_cdt->Triangulate();

		const vector<p2t::Triangle*> tris = m_cdt->GetTriangles();

		for (const auto& tri : tris)
		{
			NavigationMeshNode* node = new NavigationMeshNode;

			Triangle t =
			{
				{ tri->GetPoint(0)->x, tri->GetPoint(0)->y },
				{ tri->GetPoint(1)->x, tri->GetPoint(1)->y },
				{ tri->GetPoint(2)->x, tri->GetPoint(2)->y }
			};

			node->AddVertices(t);

			m_nodes.emplace_back(node);
		}

		/*for (const auto& node : m_nodes)
		{
			for (const auto& node2 : m_nodes)
			{
				if (node == node2)
				{
					continue;
				}

				if (vec2 v[2]; node->GetAdjacentVertices(node2, v) == 2)
				{
					const float distance = glm::length(node->position - node2->position);
					node->ConnectTo(node2, distance);
					node2->ConnectTo(node, distance);
				}
			}
		}*/

		for (auto& p : m_polygons)
		{
			for (const auto& ptr : p)
			{
				delete ptr;
			}
		}

		m_polygons.clear();

		delete m_cdt;
		m_cdt = nullptr;
	}

	Node* NavigationMesh::ClosestNode(const vec2 worldPos)
	{
		Node* closest = nullptr;
		float closestDist = FLT_MAX;

		for (const auto& node : m_nodes)
		{
			float dist = glm::distance(node->position, worldPos);
			if (dist < closestDist)
			{
				closestDist = dist;
				closest = node;
			}
		}

		return closest;
	}

	Node* NavigationMesh::RandomNode(Random* random)
	{
		return m_nodes[random->Range(0, static_cast<int>(m_nodes.size()))];
	}

	vector<Node*> NavigationMesh::SmoothPath(const vector<Node*>& path)
	{
		return { };
	}

	string NavigationMesh::DebugCategory()
	{
		return "Navigation Mesh";
	}

	void NavigationMesh::RenderDebuggingTools(Renderer* renderer, EVerbosity verbosity)
	{
		// draw nav mesh polygons
		for (const auto& node : m_nodes)
		{
			renderer->DrawLine(GlmToAie(*node->GetVertex(0)), GlmToAie(*node->GetVertex(1)), 1.f, { 0, 128, 255, 255 });
			renderer->DrawLine(GlmToAie(*node->GetVertex(1)), GlmToAie(*node->GetVertex(2)), 1.f, { 0, 128, 255, 255 });
			renderer->DrawLine(GlmToAie(*node->GetVertex(2)), GlmToAie(*node->GetVertex(0)), 1.f, { 0, 128, 255, 255 });
		}

		bool showObstacles = false;
		if (Debugger::Get("navmesh.obstacles.visible", showObstacles) && showObstacles)
		{
			// draw obstacles
			for (const auto& o : m_obstacles)
			{
				o->RenderDebuggingTools(renderer, verbosity);
			}
		}
	}

	void NavigationMesh::HandleImGui(EVerbosity verbosity)
	{
		bool showObstacles = false;
		if (!Debugger::Get("navmesh.obstacles.visible", showObstacles))
		{
			Debugger::Set("navmesh.obstacles.visible", false);
		}

		if (ImGui::Checkbox("Show Obstacles", &showObstacles))
		{
			Debugger::Set("navmesh.obstacles.visible", showObstacles);
		}
	}
}
