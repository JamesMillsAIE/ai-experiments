#pragma once

#include <functional>
#include <list>

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>

#include "Global.h"

#include "Core/IDebugHandler.h"

using std::function;
using std::list;

using glm::vec2;
using glm::vec3;

using Debugging::IDebugHandler;
using Debugging::Debugger;
using Debugging::EVerbosity;

class Renderer2D;
class Texture;

namespace Pathfinding
{
	class Edge;
	class Graph;

	class Node
	{
	public:
		static string walkableColorId;
		static string notWalkableColorId;
		static string connectionColorId;

	public:
		vec2 position;

		float gScore;
		float hScore;
		bool isWalkable;

		Node* previous;

	public:
		Node();
		~Node();

	public:
		float FScore() const;
		list<Edge*> Edges() const;

		void ConnectTo(Node* other, float cost);

		void Render(Renderer2D* renderer) const;
		void RenderConnections(Renderer2D* renderer) const;

	private:
		list<Edge*> m_edges;

	};

	class Edge
	{
	public:
		Node* endPoint;
		float cost;

	public:
		Edge(Node* endPoint, float cost);

	};

	class Graph final : public IDebugHandler
	{
		friend class AStar;

	public:
		void Build() const;
		void BuildFrom(const Texture* texture, bool isObstacleMap = false) const;
		void BuildConnections() const;

		Node* FindNode(vec2 location) const;

		uint Width() const;
		uint Height() const;
		float Spacing() const;
		bool InsideBounds(vec2 location) const;

		int GetNearbyNodes(vec2 location, list<Node*>& nodes, float searchRadius = 100.f) const;

		void ForEach(function<void(Node* node, uint x, uint y)> predicate) const;

		string DebugCategory() override;

	protected:
		void RenderDebuggingTools(Renderer2D* renderer, EVerbosity verbosity) override;
		void HandleImGui(EVerbosity verbosity) override;

	private:
		Node*** m_graph;

		uint m_width;
		uint m_height;

		float m_spacing;

	private:
		Graph(uint width, uint height, float spacing);
		~Graph() override;

	};

	typedef float(*Heuristic)(Node*, Node*, Graph*);

	class AStar
	{
	public:
		static bool FindPath(vec2 start, vec2 end, Heuristic heuristic, list<Node*>& path);

		static void InitialiseFrom(float spacing, const string& texture, bool isObstacleMap = false);

	private:
		static Graph* m_graph;

	};
}