#pragma once

#include <vector>

#include <AI/Pathfinding/PathfindingNode.h>

using std::vector;

namespace p2t
{
	struct Point;
}

using p2t::Point;

namespace Pathfinding
{
	class NavMeshNode final : public PathfindingNode
	{
	public:
		float gScore;
		float hScore;

	public:
		NavMeshNode();
		~NavMeshNode() override;

	public:
		void AddVertex(const Point* vert);
		vector<vec2> GetVertices();
		vec2 GetVertex(size_t index) const;

		void CenterOnVertices();
		int GetAdjacentVertices(const NavMeshNode* other, vec2* adjacent) const;

		float FScore() const;

	private:
		vector<vec2> m_vertices;

	};
}
