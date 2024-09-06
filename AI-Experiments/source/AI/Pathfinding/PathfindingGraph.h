#pragma once

#include <functional>
#include <iostream>

#include <Core/Debugging/IDebugHandler.h>
#include <Core/Rendering/Renderer2D.h>
#include <Core/Rendering/Texture.h>

#include <glm/geometric.hpp>
#include <glm/vec2.hpp>

#include "Global.h"
#include "imgui.h"
#include "PathfindingNode.h"

using std::function;

using glm::vec2;

using Debugging::IDebugHandler;
using Debugging::Debugger;
using Debugging::EVerbosity;

template<class T, class U>
concept derived = std::is_base_of_v<U, T>;

namespace Pathfinding
{
	template<derived<PathfindingNode> NODE_TYPE>
	class PathfindingGraph final : public IDebugHandler
	{
	public:
		PathfindingGraph(uint width, uint height, float spacing);
		~PathfindingGraph() override;

	public:
		void Build() const;
		void BuildFrom(const Texture* texture, bool isObstacleMap = false) const;
		void BuildConnections() const;

		NODE_TYPE* FindNode(vec2 location) const;

		uint Width() const;
		uint Height() const;
		float Spacing() const;
		bool InsideBounds(vec2 location) const;

		int GetNearbyNodes(vec2 location, list<NODE_TYPE*>& nodes, float searchRadius = 100.f) const;

		void ForEach(function<void(NODE_TYPE*, uint, uint)> predicate) const;

		string DebugCategory() override;

	protected:
		void RenderDebuggingTools(Renderer2D* renderer, EVerbosity verbosity) override;
		void HandleImGui(EVerbosity verbosity) override;

	private:
		NODE_TYPE*** m_graph;

		uint m_width;
		uint m_height;

		float m_spacing;

	};

	template <derived<PathfindingNode> NODE_TYPE>
	PathfindingGraph<NODE_TYPE>::PathfindingGraph(uint width, uint height, float spacing)
		: m_graph{ new NODE_TYPE**[height]}, m_width{width}, m_height{height}, m_spacing{spacing}
	{
		for (uint y = 0; y < height; ++y)
		{
			m_graph[y] = new NODE_TYPE * [width];

			for (uint x = 0; x < width; ++x)
			{
				m_graph[y][x] = nullptr;
			}
		}

		PathfindingNode::walkableColorId = MakeId(PathfindingGraph::DebugCategory(), { "node", "walkable", "color" });
		PathfindingNode::notWalkableColorId = MakeId(PathfindingGraph::DebugCategory(), { "node", "not", "walkable", "color" });
		PathfindingNode::connectionColorId = MakeId(PathfindingGraph::DebugCategory(), { "node", "connection", "color" });

		Debugger::Set(PathfindingNode::walkableColorId, vec3{ .9f, .9f, .9f });
		Debugger::Set(PathfindingNode::notWalkableColorId, vec3{ .9f, 0.f, 0.f });
		Debugger::Set(PathfindingNode::connectionColorId, vec3{ 0.f, 0.f, 0.f });
	}

	template <derived<PathfindingNode> NODE_TYPE>
	PathfindingGraph<NODE_TYPE>::~PathfindingGraph()
	{
		for (uint y = 0; y < m_height; ++y)
		{
			for (uint x = 0; x < m_width; ++x)
			{
				delete m_graph[y][x];
			}

			delete[] m_graph[y];
		}

		delete[] m_graph;
	}

	template <derived<PathfindingNode> NODE_TYPE>
	void PathfindingGraph<NODE_TYPE>::Build() const
	{
		const uint uSpacing = static_cast<uint>(m_spacing);
		const uint halfSpacing = uSpacing / 2;

		for (uint y = 0; y < m_height; ++y)
		{
			for (uint x = 0; x < m_width; ++x)
			{
				m_graph[y][x] = new NODE_TYPE({ x * uSpacing + halfSpacing, y * uSpacing + halfSpacing });
			}
		}

		BuildConnections();
	}

	template <derived<PathfindingNode> NODE_TYPE>
	void PathfindingGraph<NODE_TYPE>::BuildFrom(const Texture* texture, bool isObstacleMap) const
	{
		const uint uSpacing = static_cast<uint>(m_spacing);
		const uint halfSpacing = uSpacing / 2;

		for (uint y = 0; y < m_height; ++y)
		{
			for (uint x = 0; x < m_width; ++x)
			{
				m_graph[y][x] = new NODE_TYPE({ x * uSpacing + halfSpacing, y * uSpacing + halfSpacing });

				if (isObstacleMap && texture)
				{
					m_graph[y][x]->walkable = texture->GetPixelAt(x, y) != 0xff0000ff;
				}
			}
		}

		BuildConnections();
	}

	template <derived<PathfindingNode> NODE_TYPE>
	void PathfindingGraph<NODE_TYPE>::BuildConnections() const
	{
		ForEach([this](NODE_TYPE* node, uint, uint)
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
	NODE_TYPE* PathfindingGraph<NODE_TYPE>::FindNode(vec2 location) const
	{
		if (!InsideBounds(location))
		{
			return nullptr;
		}

		return m_graph[static_cast<uint>(location.y / m_spacing)][static_cast<uint>(location.x / m_spacing)];
	}

	template <derived<PathfindingNode> NODE_TYPE>
	uint PathfindingGraph<NODE_TYPE>::Width() const
	{
		return m_width;
	}

	template <derived<PathfindingNode> NODE_TYPE>
	uint PathfindingGraph<NODE_TYPE>::Height() const
	{
		return m_height;
	}

	template <derived<PathfindingNode> NODE_TYPE>
	float PathfindingGraph<NODE_TYPE>::Spacing() const
	{
		return m_spacing;
	}

	template <derived<PathfindingNode> NODE_TYPE>
	bool PathfindingGraph<NODE_TYPE>::InsideBounds(vec2 location) const
	{
		return location.x >= 0.f && location.x < static_cast<float>(m_width) * m_spacing &&
			location.y >= 0.f && location.y < static_cast<float>(m_height) * m_spacing;
	}

	template <derived<PathfindingNode> NODE_TYPE>
	int PathfindingGraph<NODE_TYPE>::GetNearbyNodes(vec2 location, list<NODE_TYPE*>& nodes, float searchRadius) const
	{
		if (!InsideBounds(location))
		{
			nodes.clear();
			return -1;
		}

		ForEach([&nodes, searchRadius, location](NODE_TYPE* node, uint, uint) -> void
			{
				if (glm::distance(node->position, location) < searchRadius)
				{
					nodes.emplace_back(node);
				}
			});

		return static_cast<int>(nodes.size());
	}

	template <derived<PathfindingNode> NODE_TYPE>
	void PathfindingGraph<NODE_TYPE>::ForEach(function<void(NODE_TYPE*, uint, uint)> predicate) const
	{
		for (uint y = 0; y < m_height; ++y)
		{
			for (uint x = 0; x < m_width; ++x)
			{
				std::invoke(predicate, m_graph[y][x], x, y);
			}
		}
	}

	template <derived<PathfindingNode> NODE_TYPE>
	string PathfindingGraph<NODE_TYPE>::DebugCategory()
	{
		return "Pathfinding Graph";
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

			ForEach([renderer](const PathfindingNode* node, uint, uint)
				{
					node->RenderConnections(renderer);
				});

			ForEach([renderer](const PathfindingNode* node, uint, uint)
				{
					node->Render(renderer);
				});
		}
	}

	template <derived<PathfindingNode> NODE_TYPE>
	void PathfindingGraph<NODE_TYPE>::HandleImGui(EVerbosity verbosity)
	{
		// ReSharper disable CppTooWideScopeInitStatement
		vec3 walkableColor;
		if (Debugger::Get(PathfindingNode::walkableColorId, walkableColor))
		{
			float colArray[] = { walkableColor.r, walkableColor.g, walkableColor.b };

			if (ImGui::ColorEdit3("Walkable Color", colArray))
			{
				Debugger::Set(PathfindingNode::walkableColorId, vec3(colArray[0], colArray[1], colArray[2]));
			}
		}

		vec3 notWalkableColor;
		if (Debugger::Get(PathfindingNode::notWalkableColorId, notWalkableColor))
		{
			float colArray[] = { notWalkableColor.r, notWalkableColor.g, notWalkableColor.b };

			if (ImGui::ColorEdit3("Not Walkable Color", colArray))
			{
				Debugger::Set(PathfindingNode::notWalkableColorId, vec3(colArray[0], colArray[1], colArray[2]));
			}
		}

		vec3 connectionColor;
		if (Debugger::Get(PathfindingNode::connectionColorId, connectionColor))
		{
			float colArray[] = { connectionColor.r, connectionColor.g, connectionColor.b };

			if (ImGui::ColorEdit3("Connection Color", colArray))
			{
				Debugger::Set(PathfindingNode::connectionColorId, vec3(colArray[0], colArray[1], colArray[2]));
			}
		}
		// ReSharper restore CppTooWideScopeInitStatement
	}
}

