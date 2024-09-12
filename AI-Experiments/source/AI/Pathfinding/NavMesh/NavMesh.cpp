#include "NavMesh.h"

#include <imgui.h>

#include <Core/Rendering/Renderer2D.h>
#include <Core/Structures/Random.h>

#include <glm/geometric.hpp>

#include "NavMeshNode.h"
#include "NavMeshObstacle.h"

#include "AI/Pathfinding/PathfindingEdge.h"

#include "Core/Structures/Line.h"
#include "Core/Structures/PriorityHeap.h"

using Debugging::Debugger;

using p2t::Triangle;

namespace Pathfinding
{
	struct NodeComparator
	{
	public:
		bool operator()(const NavMeshNode* a, const NavMeshNode* b) const
		{
			return a->FScore() > b->FScore();
		}

	};

	NavMesh::NavMesh(const float width, const float height)
		: m_extents{ width, height }, m_graph{ new PathfindingGraph<NavMeshNode>(1, 1, 0.f) }
	{
		float padding = 1.f;

		m_polygons.emplace_back();
		m_polygons[0].emplace_back(new Point(-padding, padding));
		m_polygons[0].emplace_back(new Point(-padding, height + padding));
		m_polygons[0].emplace_back(new Point(width + padding, height + padding));
		m_polygons[0].emplace_back(new Point(width + padding, -padding));

		m_cdt = new CDT(m_polygons[0]);
	}

	NavMesh::~NavMesh()
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

	bool NavMesh::AddObstacle(const vec2 pos, const vec2 size, const float padding)
	{
		for (const auto& ob : m_obstacles)
		{
			if (ob->Overlaps(pos, size, padding))
			{
				return false;
			}
		}

		NavMeshObstacle* o = new NavMeshObstacle(pos, size, padding);
		m_obstacles.emplace_back(o);

		m_polygons.emplace_back();
		m_polygons.back().emplace_back(new Point(pos.x - padding, pos.y - padding));
		m_polygons.back().emplace_back(new Point(pos.x - padding, pos.y + size.y + padding));
		m_polygons.back().emplace_back(new Point(pos.x + size.x + padding, pos.y + size.y + padding));
		m_polygons.back().emplace_back(new Point(pos.x + size.x + padding, pos.y - padding));
		m_cdt->AddHole(m_polygons.back());

		return true;
	}

	void NavMesh::AddRandomObstacles(const Random* random, const int num, const vec2 size, const float padding)
	{
		for (int i = 0; i < num; ++i)
		{
			bool safe;

			do
			{
				safe = AddObstacle(
					{
						random->Range(size.x, m_extents.x - size.x - padding),
						random->Range(size.y, m_extents.y - size.y - padding)
					},
					size, padding);
			} while (!safe);
		}
	}

	void NavMesh::Build()
	{
		m_cdt->Triangulate();

		const vector<Triangle*> tris = m_cdt->GetTriangles();

		for (const auto& tri : tris)
		{
			NavMeshNode* node = new NavMeshNode;

			node->AddVertex(tri->GetPoint(0));
			node->AddVertex(tri->GetPoint(1));
			node->AddVertex(tri->GetPoint(2));

			node->CenterOnVertices();

			m_nodes.emplace_back(node);
		}

		for (const auto& node : m_nodes)
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
		}

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

		m_graph->BuildFrom(m_nodes);
	}

	PathfindingNode* NavMesh::RandomNode(const Random* random) const
	{
		return m_nodes[random->Range(0, static_cast<int>(m_nodes.size()))];
	}

	PathfindingNode* NavMesh::GetClosestNode(const vec2& position) const
	{
		NavMeshNode* closest = nullptr;
		float closestDist = FLT_MAX;

		for (const auto& node : m_nodes)
		{
			float dist = glm::distance(node->position, position);
			if (dist < closestDist)
			{
				closestDist = dist;
				closest = node;
			}
		}

		return closest;
	}

	bool NavMesh::FindPath(const vec2& start, const vec2& end, vector<vec2>& path)
	{
		if (!m_graph)
		{
			return false;
		}

		NavMeshNode* startNode = m_graph->GetNearest(start);
		NavMeshNode* endNode = m_graph->GetNearest(end);

		if (!startNode || !endNode)
		{
			return false;
		}

		for (auto& node : m_nodes)
		{
			node->previous = nullptr;
		}

		priority_heap<NavMeshNode*, vector<NavMeshNode*>, NodeComparator> openList;
		list<NavMeshNode*> closedList;

		openList.push(startNode);

		bool found = false;
		while (!openList.empty())
		{
			NavMeshNode* current = openList.top();
			if (current == endNode)
			{
				found = true;
				break;
			}

			openList.pop();
			closedList.emplace_back(current);

			for (const auto& connection : current->Edges())
			{
				if (NavMeshNode* aStarNode = dynamic_cast<NavMeshNode*>(connection->endPoint))
				{
					if (std::ranges::find(closedList, connection->endPoint) == closedList.end())
					{
						const float gScore = current->gScore + connection->cost;
						const float hScore = distance(current->position, aStarNode->position);
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
			vector<NavMeshNode*> p;

			NavMeshNode* current = endNode;
			while (current)
			{
				p.emplace_back(current);

				current = dynamic_cast<NavMeshNode*>(current->previous);
			}

			std::ranges::reverse(p);

			path = SmoothPath(p, start, end);
		}

		return found;
	}

	string NavMesh::DebugCategory()
	{
		return "Nav Mesh";
	}

	void NavMesh::RenderDebuggingTools(Renderer2D* renderer, EVerbosity verbosity)
	{
		renderer->SetRenderColour({ 0.f, .5f, 1.f });

		// draw nav mesh polygons
		for (const auto& node : m_nodes)
		{
			renderer->DrawLine(node->GetVertex(0), node->GetVertex(1));
			renderer->DrawLine(node->GetVertex(1), node->GetVertex(2));
			renderer->DrawLine(node->GetVertex(2), node->GetVertex(0));
		}

		bool showObstacles = false;
		if (Debugger::Get("navmesh.obstacles.visible", showObstacles) && showObstacles)
		{
			renderer->SetRenderColour({ 1.f, 0.f, 0.f });

			// draw obstacles
			for (const auto& o : m_obstacles)
			{
				renderer->DrawBox(o->position, o->size, 0.f, 0.f, 0.f, 0.f);
			}
		}

		if (m_graph)
		{
			m_graph->RenderDebuggingTools(renderer, verbosity);
		}
	}

	void NavMesh::HandleImGui(EVerbosity verbosity)
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

		if (m_graph)
		{
			m_graph->HandleImGui(verbosity);
		}
	}

	vector<vec2> NavMesh::SmoothPath(const vector<NavMeshNode*>& path, vec2 start, vec2 end)
	{
		vector<vec2> smooth;

		if (Physics::CheckCollisionPointTriangle(end, path[0]->GetVertex(0), path[0]->GetVertex(1), path[0]->GetVertex(2)))
		{
			smooth.emplace_back(path[0]->position);

			return smooth;
		}

		vector<Line> portals;

		if (!path.empty())
		{
			NavMeshNode* prev = nullptr;

			for (auto& node : path)
			{
				if (prev != nullptr)
				{
					Line l = {};

					vec2 adj[2];
					prev->GetAdjacentVertices(node, adj);

					vec2 fromPrev =
					{
						node->position.x - prev->position.x,
						node->position.y - prev->position.y
					};

					vec2 toAdj0 =
					{
						adj[0].x - prev->position.x,
						adj[0].y - prev->position.y
					};

					if (fromPrev.x * toAdj0.y - toAdj0.x * fromPrev.y > 0)
					{
						l.start = adj[0];
						l.end = adj[1];
					}
					else
					{
						l.start = adj[1];
						l.end = adj[0];
					}

					portals.emplace_back(l);
				}
				prev = node;
			}
		}

		smooth.emplace_back(start);

		vec2 currentPoint = start;

		for (Line& l : portals)
		{
			vec2* point = l.DoesIntersect({ currentPoint,  end });

			if (point)
			{
				currentPoint = *point;
			}
			else
			{
				currentPoint = l.GetClosestPoint((currentPoint + end) / 2.0f);
			}

			smooth.emplace_back(currentPoint);
		}

		smooth.emplace_back(end);

		return smooth;
	}
}
