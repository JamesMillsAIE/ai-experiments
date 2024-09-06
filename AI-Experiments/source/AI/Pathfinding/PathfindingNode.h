#pragma once

#include <list>
#include <string>

#include <glm/vec2.hpp>

using std::list;
using std::string;

using glm::vec2;

class Renderer2D;

namespace Pathfinding
{
	class PathfindingEdge;

	class PathfindingNode
	{
	public:
		static string walkableColorId;
		static string notWalkableColorId;
		static string connectionColorId;

	public:
		vec2 position;
		bool walkable;

		PathfindingNode* previous;

	public:
		explicit PathfindingNode(vec2 position);
		virtual ~PathfindingNode();

	public:
		list<PathfindingEdge*> Edges() const;

		void ConnectTo(PathfindingNode* other, float cost);

		void Render(Renderer2D* renderer) const;
		void RenderConnections(Renderer2D* renderer) const;

	private:
		list<PathfindingEdge*> m_edges;

	};
}