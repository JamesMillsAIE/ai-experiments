#pragma once

#include <functional>
#include <imgui.h>
#include <iostream>
#include <vector>

#include <Core/Debugging/IDebugHandler.h>
#include <Core/Debugging/ImGuiTools.h>
#include <Core/Rendering/Renderer2D.h>
#include <Core/Rendering/Texture.h>

#include <glm/geometric.hpp>
#include <glm/vec2.hpp>

#include "Global.h"
#include "PathfindingNode.h"

#include "Core/Physics.h"

using std::function;
using std::vector;

using glm::vec2;

using Debugging::IDebugHandler;
using Debugging::Debugger;
using Debugging::ImGuiTools;
using Debugging::EVerbosity;

template<class T, class U>
concept derived = std::is_base_of_v<U, T>;

namespace Pathfinding
{
	template<derived<PathfindingNode> NODE_TYPE>
	class PathfindingGraph
	{
	public:
		PathfindingGraph(uint width, uint height, float spacing);
		~PathfindingGraph();

	public:
		void BuildFrom(const Texture* texture, bool isObstacleMap = false);
		void BuildFrom(vector<NODE_TYPE*> nodes);
		void BuildConnections() const;

		NODE_TYPE* GetNearest(const vec2& position);

		float Spacing() const;
		int GetNearbyNodes(vec2 location, list<NODE_TYPE*>& nodes, float searchRadius = 100.f) const;

		void ForEach(function<void(NODE_TYPE*)> predicate) const;

		void RenderDebuggingTools(Renderer2D* renderer, EVerbosity verbosity);
		void HandleImGui(EVerbosity verbosity);

	private:
		vector<NODE_TYPE*> m_graph;

		float m_spacing;

	};

	template <derived<PathfindingNode> NODE_TYPE>
	PathfindingGraph<NODE_TYPE>::PathfindingGraph(uint width, uint height, float spacing)
		: m_spacing{ spacing }
	{
		m_graph.reserve(height * width);

		for (uint y = 0; y < height; ++y)
		{
			for (uint x = 0; x < width; ++x)
			{
				m_graph.emplace_back(nullptr);
			}
		}

		PathfindingNode::walkableColorId = MakeId("Pathfinding", { "node", "walkable", "color" });
		PathfindingNode::notWalkableColorId = MakeId("Pathfinding", { "node", "not", "walkable", "color" });
		PathfindingNode::connectionColorId = MakeId("Pathfinding", { "node", "connection", "color" });

		Debugger::Set(PathfindingNode::walkableColorId, vec3{ .9f, .9f, .9f });
		Debugger::Set(PathfindingNode::notWalkableColorId, vec3{ .9f, 0.f, 0.f });
		Debugger::Set(PathfindingNode::connectionColorId, vec3{ 0.f, 0.f, 0.f });
	}

	template <derived<PathfindingNode> NODE_TYPE>
	PathfindingGraph<NODE_TYPE>::~PathfindingGraph()
	{
		for (const auto& node : m_graph)
		{
			delete node;
		}
	}

	template <derived<PathfindingNode> NODE_TYPE>
	void PathfindingGraph<NODE_TYPE>::BuildFrom(const Texture* texture, bool isObstacleMap)
	{
		const uint uSpacing = static_cast<uint>(m_spacing);
		const uint halfSpacing = uSpacing / 2;

		for (uint y = 0; y < texture->GetHeight(); ++y)
		{
			for (uint x = 0; x < texture->GetWidth(); ++x)
			{
				m_graph[y * texture->GetWidth() + x] = new NODE_TYPE({ x * uSpacing + halfSpacing, y * uSpacing + halfSpacing });

				if (isObstacleMap && texture)
				{
					m_graph[y * texture->GetWidth() + x]->walkable = texture->GetPixelAt(x, y) != 0xff0000ff;
				}
			}
		}

		BuildConnections();
	}

	template <derived<PathfindingNode> NODE_TYPE>
	void PathfindingGraph<NODE_TYPE>::BuildFrom(vector<NODE_TYPE*> nodes)
	{
		for (const auto& node : m_graph)
		{
			delete node;
		}

		m_graph.clear();
		m_graph = nodes;
	}

	template <derived<PathfindingNode> NODE_TYPE>
	void PathfindingGraph<NODE_TYPE>::BuildConnections() const
	{
		ForEach([this](NODE_TYPE* node)
			{
				if (!node->walkable)
				{
					return;
				}

				list<NODE_TYPE*> nearbyNodes;
				if (GetNearbyNodes(node->position, nearbyNodes, 40.f) != -1)
				{
					for (const auto& nearby : nearbyNodes)
					{
						if (nearby->walkable)
						{
							node->ConnectTo(nearby, 1.f);
						}
					}
				}
			});
	}

	template <derived<PathfindingNode> NODE_TYPE>
	NODE_TYPE* PathfindingGraph<NODE_TYPE>::GetNearest(const vec2& position)
	{
		NODE_TYPE* closestNode = nullptr;
		float closestDist = FLT_MAX;

		for (const auto& node : m_graph)
		{
			float dist = glm::distance(node->position, position);
			if (dist < closestDist)
			{
				closestNode = node;
				closestDist = dist;
			}
		}

		return closestNode;
	}

	template <derived<PathfindingNode> NODE_TYPE>
	float PathfindingGraph<NODE_TYPE>::Spacing() const
	{
		return m_spacing;
	}

	template <derived<PathfindingNode> NODE_TYPE>
	int PathfindingGraph<NODE_TYPE>::GetNearbyNodes(vec2 location, list<NODE_TYPE*>& nodes, float searchRadius) const
	{
		ForEach([&nodes, searchRadius, location](NODE_TYPE* node) -> void
			{
				if (glm::distance(node->position, location) < searchRadius)
				{
					nodes.emplace_back(node);
				}
			});

		return static_cast<int>(nodes.size());
	}

	template <derived<PathfindingNode> NODE_TYPE>
	void PathfindingGraph<NODE_TYPE>::ForEach(function<void(NODE_TYPE*)> predicate) const
	{
		for (const auto& node : m_graph)
		{
			predicate(node);
		}
	}

	template <derived<PathfindingNode> NODE_TYPE>
	void PathfindingGraph<NODE_TYPE>::RenderDebuggingTools(Renderer2D* renderer, EVerbosity verbosity)
	{
		if (verbosity == EVerbosity::Advanced)
		{
			vec3 connectionColor = { .9f, .9f, .9f };
			if (!Debugger::Get(PathfindingNode::connectionColorId, connectionColor))
			{
				std::cout << PathfindingNode::connectionColorId << " not found!\n";
			}

			renderer->SetRenderColour(connectionColor);

			ForEach([renderer](const PathfindingNode* node)
				{
					node->RenderConnections(renderer);
				});

			ForEach([renderer](const PathfindingNode* node)
				{
					node->Render(renderer);
				});
		}
	}

	template <derived<PathfindingNode> NODE_TYPE>
	void PathfindingGraph<NODE_TYPE>::HandleImGui(EVerbosity verbosity)
	{
		if (vec3 walkableColor; Debugger::Get(PathfindingNode::walkableColorId, walkableColor))
		{
			if (ImGuiTools::ColorEditField("Walkable Color", walkableColor))
			{
				Debugger::Set(PathfindingNode::walkableColorId, walkableColor);
			}
		}

		if (vec3 notWalkableColor; Debugger::Get(PathfindingNode::notWalkableColorId, notWalkableColor))
		{
			if (ImGuiTools::ColorEditField("Not Walkable Color", notWalkableColor))
			{
				Debugger::Set(PathfindingNode::notWalkableColorId, notWalkableColor);
			}
		}

		if (vec3 connectionColor; Debugger::Get(PathfindingNode::connectionColorId, connectionColor))
		{
			if (ImGuiTools::ColorEditField("Connection Color", connectionColor))
			{
				Debugger::Set(PathfindingNode::connectionColorId, connectionColor);
			}
		}
	}
}

