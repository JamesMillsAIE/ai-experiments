#pragma once

#include <list>

#include <glm/vec2.hpp>

#include "Global.h"

using std::list;

using glm::vec;

typedef vec<2, uint> vec2i;

namespace Pathfinding
{
	class Edge;

	class Node
	{
	public:
		vec2i position;

		float gScore;
		float hScore;

		Node* previous;

	public:
		Node();
		~Node();

	public:
		float FScore() const;
		list<Edge*> Edges() const;

		void ConnectTo(Node* other, float cost);

	private:
		list<Edge*> m_edges;

	};

	class Edge
	{
	public:
		Edge(Node* endPoint, float cost);

	public:
		Node* EndPoint() const;
		float Cost() const;

	private:
		Node* m_endPoint;
		float m_cost;

	};

	class Graph
	{
	public:
		Graph(uint width, uint height, float spacing);
		~Graph();

	public:
		void Build();

		Node* FindNode(uint x, uint y) const;

		uint Width() const;
		uint Height() const;
		float Spacing() const;

	private:
		Node*** m_graph;

		uint m_width;
		uint m_height;

		float m_spacing;

	};

	typedef float(*Heuristic)(Node*, Node*, Graph*);

	class AStar
	{
	public:
		static bool FindPath(vec2i start, vec2i end, Graph* graph, Heuristic heuristic, list<Node*>& path);

	};
}