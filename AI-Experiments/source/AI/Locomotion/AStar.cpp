#include "AStar.h"

#include <Core/PriorityHeap.h>

namespace Pathfinding
{
	Node::Node()
		: position{ 0, 0 }, gScore{ 0 }, hScore{ 0 }, previous{ nullptr }
	{
	}

	Node::~Node()
	{
		for(auto& connection : m_edges)
		{
			delete connection;
		}
	}

	float Node::FScore() const
	{
		return gScore + hScore;
	}

	list<Edge*> Node::Edges() const
	{
		return m_edges;
	}

	void Node::ConnectTo(Node* other, const float cost)
	{
		m_edges.emplace_back(new Edge(other, cost));
	}

	Edge::Edge(Node* endPoint, const float cost)
		: m_endPoint{ endPoint }, m_cost{ cost }
	{
	}

	Node* Edge::EndPoint() const
	{
		return m_endPoint;
	}

	float Edge::Cost() const
	{
		return m_cost;
	}

	Graph::Graph(const uint width, const uint height, const float spacing)
		: m_graph{ new Node**[height] }, m_width{ width }, m_height{ height }, m_spacing{ spacing }
	{
		for(uint y = 0; y < m_height; ++y)
		{
			m_graph[y] = new Node * [m_width];

			for (uint x = 0; x < m_width; ++x)
			{
				m_graph[y][x] = nullptr;
			}
		}
	}

	Graph::~Graph()
	{
		for(uint y = 0; y < m_height; ++y)
		{
			for(uint x = 0; x < m_width; ++x)
			{
				delete m_graph[y][x];
			}

			delete[] m_graph[y];
		}

		delete[] m_graph;
	}

	void Graph::Build()
	{
		for (uint y = 0; y < m_height; ++y)
		{
			for (uint x = 0; x < m_width; ++x)
			{
				if (rand() % 4 > 1)
				{
					m_graph[y][x] = new Node;
					m_graph[y][x]->position = { x, y };
				}
			}
		}
	}

	Node* Graph::FindNode(const uint x, const uint y) const
	{
		if(x < 0 || x > m_width || y < 0 || y > m_height)
		{
			return nullptr;
		}

		return m_graph[y][x];
	}

	uint Graph::Width() const
	{
		return m_width;
	}

	uint Graph::Height() const
	{
		return m_height;
	}

	float Graph::Spacing() const
	{
		return m_spacing;
	}

	struct NodeComparator
	{
	public:
		bool operator()(const Node* a, const Node* b) const
		{
			return a->FScore() > b->FScore();
		}

	};

	bool AStar::FindPath(const vec2i start, const vec2i end, Graph* graph,
	                     const Heuristic heuristic, list<Node*>& path)
	{
		if(!graph)
		{
			return false;
		}

		Node* startNode = graph->FindNode(start.x, start.y);
		Node* endNode = graph->FindNode(end.x, end.y);

		if(!startNode || !endNode)
		{
			return false;
		}

		priority_heap<Node*, vector<Node*>, NodeComparator> openList;
		list<Node*> closedList;

		openList.push(startNode);

		bool found = false;
		while(!openList.empty())
		{
			Node* current = openList.top();
			if(current == endNode)
			{
				found = true;
				break;
			}

			openList.pop();
			closedList.emplace_back(current);

			for(auto& connection : current->Edges())
			{
				if(std::ranges::find(closedList, connection->EndPoint()) == closedList.end())
				{
					float gScore = current->gScore + connection->Cost();
					float hScore = heuristic(current, connection->EndPoint(), graph);
					float fScore = gScore + hScore;

					if(openList.find(connection->EndPoint()) == openList.end())
					{
						connection->EndPoint()->gScore = gScore;
						connection->EndPoint()->hScore = hScore;
						connection->EndPoint()->previous = current;

						openList.push(connection->EndPoint());
					}
					else if(fScore < connection->EndPoint()->FScore())
					{
						connection->EndPoint()->gScore = gScore;
						connection->EndPoint()->previous = current;
					}
				}
			}
		}

		if(found)
		{
			Node* current = endNode;
			while(current)
			{
				path.emplace_back(current);

				current = current->previous;
			}

			std::ranges::reverse(path);
		}

		return found;
	}
}
