#pragma once

#include <list>

#include <glm/vec2.hpp>

using std::list;

using glm::vec2;

namespace Pathfinding
{
	class Edge;

	class Node
	{
	public:
		vec2 position;
		int flags;

	public:
		explicit Node(vec2 position);
		virtual ~Node();

	public:
		list<Edge*> Edges() const;

		void ConnectTo(Node* other, float cost);

	private:
		list<Edge*> m_edges;

	};
}