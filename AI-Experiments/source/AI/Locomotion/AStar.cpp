#include "AStar.h"

#include <Core/PriorityHeap.h>

#include <glm/detail/func_geometric.inl>

#include "Core/Renderer2D.h"
#include "Core/Texture.h"

namespace Pathfinding
{
	Node::Node()
		: position{ 0, 0 }, gScore{ 0 }, hScore{ 0 }, isWalkable{ true }, previous{ nullptr }
	{
	}

	Node::~Node()
	{
		for (auto& connection : m_edges)
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

	void Node::Render(Renderer2D* renderer) const
	{
		if (isWalkable)
		{
			renderer->SetRenderColour(.9f, .9f, .9f, 1.f);
		}
		else
		{
			renderer->SetRenderColour(.9f, 0.f, 0.f, 1.f);
		}

		renderer->DrawCircle(position.x, position.y, 6.f);

		/*if (isWalkable)
		{
			renderer->SetRenderColour(.5f, .5f, .5f, 1.f);
		}
		else
		{
			renderer->SetRenderColour(.5f, 0.f, 0.f, 1.f);
		}

		renderer->DrawCircleLines(position.x, position.y, 6.f);*/
	}

	void Node::RenderConnections(Renderer2D* renderer) const
	{
		for (const auto& connection : m_edges)
		{
			renderer->DrawLine(position.x, position.y, connection->endPoint->position.x, connection->endPoint->position.y);
		}
	}

	Edge::Edge(Node* endPoint, const float cost)
		: endPoint{ endPoint }, cost{ cost }
	{
	}

	Graph::Graph(const uint width, const uint height, const float spacing)
		: m_graph{ new Node * *[height] }, m_width{ width }, m_height{ height }, m_spacing{ spacing }
	{
		for (uint y = 0; y < m_height; ++y)
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

	void Graph::Build() const
	{
		const uint uSpacing = static_cast<uint>(m_spacing);
		const uint halfSpacing = uSpacing / 2;

		for (uint y = 0; y < m_height; ++y)
		{
			for (uint x = 0; x < m_width; ++x)
			{
				m_graph[y][x] = new Node;
				m_graph[y][x]->position = { x * uSpacing + halfSpacing, y * uSpacing + halfSpacing };
			}
		}

		BuildConnections();
	}

	void Graph::BuildFrom(const Texture* texture, const bool isObstacleMap) const
	{
		const uint uSpacing = static_cast<uint>(m_spacing);
		const uint halfSpacing = uSpacing / 2;

		for (uint y = 0; y < m_height; ++y)
		{
			for (uint x = 0; x < m_width; ++x)
			{
				m_graph[y][x] = new Node;
				m_graph[y][x]->position = { x * uSpacing + halfSpacing, y * uSpacing + halfSpacing };

				if (isObstacleMap && texture)
				{
					m_graph[y][x]->isWalkable = texture->GetPixelAt(x, y) != 0xff0000ff;
				}
			}
		}

		BuildConnections();
	}

	void Graph::BuildConnections() const
	{
		ForEach([this](Node* node, uint, uint)
			{
				if (!node->isWalkable)
				{
					return;
				}

				list<Node*> nearbyNodes;
				if (GetNearbyNodes(node->position, nearbyNodes, 40.f) != -1)
				{
					for (const auto& nearby : nearbyNodes)
					{
						if (nearby->isWalkable)
						{
							node->ConnectTo(nearby, 1.f);
						}
					}
				}
			});
	}

	Node* Graph::FindNode(const vec2 location) const
	{
		if (!InsideBounds(location))
		{
			return nullptr;
		}

		return m_graph[static_cast<uint>(location.y)][static_cast<uint>(location.x)];
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

	bool Graph::InsideBounds(vec2 location) const
	{
		return location.x >= 0.f && location.x < static_cast<float>(m_width) * m_spacing &&
			location.y >= 0.f && location.y < static_cast<float>(m_height) * m_spacing;
	}

	int Graph::GetNearbyNodes(vec2 location, list<Node*>& nodes, float searchRadius) const
	{
		if (!InsideBounds(location))
		{
			nodes.clear();
			return -1;
		}

		ForEach([&nodes, searchRadius, location](Node* node, uint, uint) -> void
			{
				if (glm::distance(node->position, location) < searchRadius)
				{
					nodes.emplace_back(node);
				}
			});

		return static_cast<int>(nodes.size());
	}

	void Graph::ForEach(function<void(Node* node, uint x, uint y)> predicate) const
	{
		for (uint y = 0; y < m_height; ++y)
		{
			for (uint x = 0; x < m_width; ++x)
			{
				std::invoke(predicate, m_graph[y][x], x, y);
			}
		}
	}

	void Graph::OnRenderDebuggingTools(Renderer2D* renderer, const EVerbosity verbosity)
	{
		if(verbosity == EVerbosity::Advanced)
		{
			renderer->SetRenderColour(.1f, .1f, .1f);

			ForEach([renderer](const Node* node, uint, uint)
				{
					node->RenderConnections(renderer);
				});

			ForEach([renderer](const Node* node, uint, uint)
				{
					node->Render(renderer);
				});
		}
	}

	struct NodeComparator
	{
	public:
		bool operator()(const Node* a, const Node* b) const
		{
			return a->FScore() > b->FScore();
		}

	};

	bool AStar::FindPath(const vec2 start, const vec2 end, Graph* graph,
		const Heuristic heuristic, list<Node*>& path)
	{
		if (!graph)
		{
			return false;
		}

		Node* startNode = graph->FindNode(start);
		Node* endNode = graph->FindNode(end);

		if (!startNode || !endNode)
		{
			return false;
		}

		priority_heap<Node*, vector<Node*>, NodeComparator> openList;
		list<Node*> closedList;

		openList.push(startNode);

		bool found = false;
		while (!openList.empty())
		{
			Node* current = openList.top();
			if (current == endNode)
			{
				found = true;
				break;
			}

			openList.pop();
			closedList.emplace_back(current);

			for (const auto& connection : current->Edges())
			{
				if (std::ranges::find(closedList, connection->endPoint) == closedList.end())
				{
					const float gScore = current->gScore + connection->cost;
					const float hScore = heuristic(current, connection->endPoint, graph);
					const float fScore = gScore + hScore;

					if (openList.find(connection->endPoint) == openList.end())
					{
						connection->endPoint->gScore = gScore;
						connection->endPoint->hScore = hScore;
						connection->endPoint->previous = current;

						openList.push(connection->endPoint);
					}
					else if (fScore < connection->endPoint->FScore())
					{
						connection->endPoint->gScore = gScore;
						connection->endPoint->previous = current;
					}
				}
			}
		}

		if (found)
		{
			Node* current = endNode;
			while (current)
			{
				path.emplace_back(current);

				current = current->previous;
			}

			std::ranges::reverse(path);
		}

		return found;
	}
}
