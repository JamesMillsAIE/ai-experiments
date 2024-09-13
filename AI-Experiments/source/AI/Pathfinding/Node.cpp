#include "Node.h"

#include "Edge.h"

namespace Pathfinding
{
	Node::Node(const vec2 position)
		: position{ position }, flags{ -1 }, previous{ nullptr }
	{
	}

	Node::~Node()
	{
		for (const auto& connection : m_edges)
		{
			delete connection;
		}

		m_edges.clear();
	}

	list<Edge*> Node::Edges() const
	{
		return m_edges;
	}

	void Node::ConnectTo(Node* other, const float cost)
	{
		m_edges.emplace_back(new Edge(other, cost));
	}
}
