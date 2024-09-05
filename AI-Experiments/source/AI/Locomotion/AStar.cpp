#include "AStar.h"

#include <iostream>

#include <Core/Rendering/Renderer2D.h>
#include <Core/Rendering/Texture.h>
#include <Core/Structures/PriorityHeap.h>

#include <glm/detail/func_geometric.inl>

#include <imgui.h>

namespace Pathfinding
{
	string Node::walkableColorId;
	string Node::notWalkableColorId;
	string Node::connectionColorId;

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

		if (isWalkable)
		{
			renderer->SetRenderColour(walkableColor);
		}
		else
		{
			renderer->SetRenderColour(notWalkableColor);
		}

		renderer->DrawCircle(position, 6.f);
	}

	void Node::RenderConnections(Renderer2D* renderer) const
	{
		for (const auto& connection : m_edges)
		{
			renderer->DrawLine(position, connection->endPoint->position);
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

		Node::walkableColorId = MakeId(Graph::DebugCategory(), { "node", "walkable", "color" });
		Node::notWalkableColorId = MakeId(Graph::DebugCategory(), { "node", "not", "walkable", "color" });
		Node::connectionColorId = MakeId(Graph::DebugCategory(), { "node", "connection", "color" });

		Debugger::Set(Node::walkableColorId, vec3{ .9f, .9f, .9f });
		Debugger::Set(Node::notWalkableColorId, vec3{ .9f, 0.f, 0.f });
		Debugger::Set(Node::connectionColorId, vec3{ 0.f, 0.f, 0.f });
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

		return m_graph[static_cast<uint>(location.y / m_spacing)][static_cast<uint>(location.x / m_spacing)];
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

	string Graph::DebugCategory()
	{
		return "AStar";
	}

	void Graph::RenderDebuggingTools(Renderer2D* renderer, const EVerbosity verbosity)
	{
		if (verbosity == EVerbosity::Advanced)
		{
			vec3 connectionColor = { .9f, .9f, .9f };
			if (!Debugger::Get(Node::connectionColorId, connectionColor))
			{
				std::cout << Node::connectionColorId << " not found!\n";
			}

			renderer->SetRenderColour(connectionColor);

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

	void Graph::HandleImGui(EVerbosity verbosity)
	{
		// ReSharper disable CppTooWideScopeInitStatement
		vec3 walkableColor;
		if (Debugger::Get(Node::walkableColorId, walkableColor))
		{
			float colArray[] = { walkableColor.r, walkableColor.g, walkableColor.b };

			if (ImGui::ColorEdit3("Walkable Color", colArray))
			{
				Debugger::Set(Node::walkableColorId, vec3(colArray[0], colArray[1], colArray[2]));
			}
		}

		vec3 notWalkableColor;
		if (Debugger::Get(Node::notWalkableColorId, notWalkableColor))
		{
			float colArray[] = { notWalkableColor.r, notWalkableColor.g, notWalkableColor.b };

			if (ImGui::ColorEdit3("Not Walkable Color", colArray))
			{
				Debugger::Set(Node::notWalkableColorId, vec3(colArray[0], colArray[1], colArray[2]));
			}
		}

		vec3 connectionColor;
		if (Debugger::Get(Node::connectionColorId, connectionColor))
		{
			float colArray[] = { connectionColor.r, connectionColor.g, connectionColor.b };

			if (ImGui::ColorEdit3("Connection Color", colArray))
			{
				Debugger::Set(Node::connectionColorId, vec3(colArray[0], colArray[1], colArray[2]));
			}
		}
		// ReSharper restore CppTooWideScopeInitStatement
	}

	Graph* AStar::m_graph = nullptr;

	struct NodeComparator
	{
	public:
		bool operator()(const Node* a, const Node* b) const
		{
			return a->FScore() > b->FScore();
		}

	};

	bool AStar::FindPath(const vec2 start, const vec2 end,const Heuristic heuristic, list<Node*>& path)
	{
		if (!m_graph)
		{
			return false;
		}

		Node* startNode = m_graph->FindNode(start);
		Node* endNode = m_graph->FindNode(end);

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
					const float hScore = heuristic(current, connection->endPoint, m_graph);
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
			path.clear();

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

	void AStar::InitialiseFrom(float spacing, const string& texture, bool isObstacleMap)
	{
		if (!m_graph)
		{
			Texture* nodeMap = new Texture(texture);

			m_graph = new Graph(nodeMap->GetWidth(), nodeMap->GetHeight(), spacing);
			m_graph->BuildFrom(nodeMap, isObstacleMap);

			delete nodeMap;
		}
	}
}
