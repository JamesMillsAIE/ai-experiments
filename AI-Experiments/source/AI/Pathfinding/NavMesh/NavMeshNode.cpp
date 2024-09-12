#include "NavMeshNode.h"

#include "common/shapes.h"

namespace Pathfinding
{
	NavMeshNode::NavMeshNode()
		: PathfindingNode{ vec2(0) }, gScore{ 0 }, hScore{ 0 }
	{
		walkable = true;
	}

	NavMeshNode::~NavMeshNode() = default;

	void NavMeshNode::AddVertex(const Point* vert)
	{
		m_vertices.emplace_back(static_cast<float>(vert->x), static_cast<float>(vert->y));
	}

	vector<vec2> NavMeshNode::GetVertices()
	{
		return m_vertices;
	}

	vec2 NavMeshNode::GetVertex(const size_t index) const
	{
		if (index >= m_vertices.size())
		{
			return {};
		}

		return m_vertices[index];
	}

	void NavMeshNode::CenterOnVertices()
	{
		for (const auto& vert : m_vertices)
		{
			position += vert;
		}

		position /= m_vertices.size();
	}

	int NavMeshNode::GetAdjacentVertices(const NavMeshNode* other, vec2* adjacent) const
	{
		int count = 0;
		for (const auto v : m_vertices)
		{
			for (const auto v2 : other->m_vertices)
			{
				if (v.x == v2.x && v.y == v2.y)
				{
					adjacent[count++] = v;
					break;
				}
			}
		}
		return count;
	}

	float NavMeshNode::FScore() const
	{
		return hScore + gScore;
	}
}
