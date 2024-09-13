#pragma once

namespace Pathfinding
{
	class Node;

	class Edge
	{
	public:
		Node* endpoint;
		float cost;

	public:
		Edge(Node* endpoint, float cost);
		virtual ~Edge();

	};
}
