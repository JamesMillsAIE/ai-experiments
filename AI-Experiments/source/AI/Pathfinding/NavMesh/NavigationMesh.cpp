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
	NavigationMesh::NavigationMesh(const float width, const float height, int maxPathSmoothing)
		: m_extents{ width, height }, m_maxPathSmoothing{ maxPathSmoothing }
	{
		m_polygons.emplace_back();
		m_polygons[0].emplace_back(new Point(0.f, 0.f));
		m_polygons[0].emplace_back(new Point(0.f, height));
		m_polygons[0].emplace_back(new Point(width, height));
		m_polygons[0].emplace_back(new Point(width, 0.f));

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
		for (const auto& ob : m_obstacles)
		{
			if (ob->Intersects(obstacle))
			{
				return false;
			}
		}

		m_obstacles.emplace_back(obstacle);

		m_polygons.emplace_back();
		for (const auto& v : obstacle->GetPaddedVertices())
		{
			m_polygons.back().emplace_back(new Point(v.x, v.y));
		}

		m_cdt->AddHole(m_polygons.back());

		return true;
	}

	void NavigationMesh::AddRandomObstacles(int num, float width, float height, const Random* random)
	{
		// random obstacles
		for (int i = 0; i < num; ++i)
		{
			do
			{
				float x = (random->Range(0.f, 1.f) * .75f + .125f) * m_extents.x;
				float y = (random->Range(0.f, 1.f) * .75f + .125f) * m_extents.y;

				NavigationObstacle* newObstacle = new NavigationObstacle(10.f);
				newObstacle->AddVertex({ x, y });
				newObstacle->AddVertex({ x, y + height });
				newObstacle->AddVertex({ x + width, y + height });
				newObstacle->AddVertex({ x + width, y });

				newObstacle->Build(EObstacleBuildFlags::AddPaddingToHull);

				if (const bool safe = AddObstacle(newObstacle); !safe)
				{
					delete newObstacle;
					break;
				}

			} while (true);
		}
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

	vector<Node*> NavigationMesh::GetNodes()
	{
		return { m_nodes.size(), dynamic_cast<Node*>(*m_nodes.data()) };
	}

	vector<vec2> NavigationMesh::SmoothPath(const vector<Node*>& path, vec2* start, vec2* end)
	{
		vector<vec2> smoothed;

		if (path.empty())
		{
			if (start != nullptr)
			{
				smoothed.emplace_back(*start);
			}

			if (end != nullptr)
			{
				smoothed.emplace_back(*end);
			}

			return smoothed;
		}

		vec2* portals = new vec2[(path.size() + 1) * 2];
		int index = 0;

		if(start != nullptr)
		{
			portals[index++] = *start;
		}
		else
		{
			portals[index++] = path.front()->position;
		}

		portals[index++] = path.front()->position;

		NavigationMeshNode* prev = nullptr;
		for (auto& n : path)
		{
			NavigationMeshNode* node = dynamic_cast<NavigationMeshNode*>(n);

			if (prev)
			{
				vec2 adj[2];
				prev->GetAdjacentVertices(node, adj);

				const vec2 fromPrev =
				{
					node->position.x - prev->position.x,
					node->position.y - prev->position.y
				};

				const vec2 toAdj0 =
				{
					adj[0].x - prev->position.x,
					adj[0].y - prev->position.y
				};

				if (fromPrev.x * toAdj0.y - toAdj0.x * fromPrev.y > 0)
				{
					portals[index++] = adj[0];
					portals[index++] = adj[1];
				}
				else
				{
					portals[index++] = adj[1];
					portals[index++] = adj[0];
				}
			}

			prev = node;
		}

		portals[index++] = path.back()->position;
		if (end != nullptr)
		{
			portals[index++] = *end;
		}
		else
		{
			portals[index++] = path.back()->position;
		}

		vec2* out = new vec2[m_maxPathSmoothing];
		const int count = StringPull(portals, index / 2, out, m_maxPathSmoothing);

		smoothed.reserve(count);

		for (int i = 0; i < count; ++i)
		{
			smoothed.emplace_back(out[i]);
		}

		delete[] portals;
		delete[] out;

		if (end != nullptr)
		{
			smoothed.emplace_back(*end);
		}

		return smoothed;
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

	int NavigationMesh::StringPull(const vec2* portals, const int numPortals, vec2* pts, const int maxPoints)
	{
		int numPoints = 0;

		vec2 portalApex = portals[0], portalLeft = portals[0], portalRight = portals[1];

		pts[numPoints++] = portalApex;

		auto triArea = [](vec2 a, vec2 b, vec2 c)
			{
				return Triangle(a, b, c).Area();
			};

		for (int i = 1; i < numPortals && numPoints < maxPoints; ++i)
		{
			const vec2 left = portals[i * 2];  // NOLINT(bugprone-implicit-widening-of-multiplication-result)
			const vec2 right = portals[i * 2 + 1];  // NOLINT(bugprone-implicit-widening-of-multiplication-result)

			if (triArea(portalApex, portalRight, right) <= 0.f)
			{
				if (portalApex == portalRight || triArea(portalApex, portalLeft, right) > 0.f)
				{
					portalRight = right;
				}
				else
				{
					pts[numPoints++] = portalLeft;

					portalApex = portalLeft;

					portalLeft = portalApex;
					portalRight = portalApex;

					continue;
				}
			}

			if(triArea(portalApex, portalLeft, left) >= 0.f)
			{
				if(portalApex == portalLeft || triArea(portalApex, portalRight, left) < 0.f)
				{
					portalLeft = left;
				}
				else
				{
					pts[numPoints++] = portalRight;

					portalApex = portalRight;

					portalLeft = portalApex;
					portalRight = portalApex;
				}
			}
		}

		if(numPoints < maxPoints)
		{
			pts[numPoints] = portals[numPortals - 1];
		}

		return numPoints;
	}

	bool NavigationMesh::IsVisibleFrom(const vec2 start, const vec2 end) const
	{
		// Does this line collide with one of the sides of the obstacle
		for (auto& obstacle : m_obstacles)
		{
			// if any of the line collision detections return true, there was a collision, the ray is not clear
			if (obstacle->Intersects(Line(start, end)))
			{
				return false;
			}
		}

		return true;
	}
}
