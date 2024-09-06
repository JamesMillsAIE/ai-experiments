#include "PathfindingNode.h"

#include <iostream>

#include <Core/Debugging/Debugger.h>
#include <Core/Rendering/Renderer2D.h>

#include "PathfindingEdge.h"

using Debugging::Debugger;

namespace Pathfinding
{
	string PathfindingNode::walkableColorId;
	string PathfindingNode::notWalkableColorId;
	string PathfindingNode::connectionColorId;

	PathfindingNode::PathfindingNode(const vec2 position)
		: position{ position }, walkable{ false }, previous{ nullptr }
	{
	}

	PathfindingNode::~PathfindingNode()
	{
		for (const auto& connection : m_edges)
		{
			delete connection;
		}
	}

	list<PathfindingEdge*> PathfindingNode::Edges() const
	{
		return m_edges;
	}

	void PathfindingNode::ConnectTo(PathfindingNode* other, float cost)
	{
		m_edges.emplace_back(new PathfindingEdge(other, cost));
	}

	void PathfindingNode::Render(Renderer2D* renderer) const
	{
		vec3 walkableColor = { .9f, .9f, .9f };
		if (!Debugger::Get(walkableColorId, walkableColor))
		{
			std::cout << walkableColorId << " not found!\n";
		}

		vec3 notWalkableColor = { .9f, 0.f, 0.f };
		if (!Debugger::Get(notWalkableColorId, notWalkableColor))
		{
			std::cout << notWalkableColorId << " not found!\n";
		}

		if (walkable)
		{
			renderer->SetRenderColour(walkableColor);
		}
		else
		{
			renderer->SetRenderColour(notWalkableColor);
		}

		renderer->DrawCircle(position, 6.f);
	}

	void PathfindingNode::RenderConnections(Renderer2D* renderer) const
	{
		for (const auto& connection : m_edges)
		{
			renderer->DrawLine(position, connection->endPoint->position);
		}
	}
}
