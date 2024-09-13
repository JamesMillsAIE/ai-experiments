#include "NavigationMeshNode.h"

#include "Physics/Shapes/Triangle.h"

namespace Pathfinding
{
	NavigationMeshNode::NavigationMeshNode()
		: Node{ vec2(0) }
	{
	}

	void NavigationMeshNode::AddVertices(Triangle& tri)
	{
		m_vertices.emplace_back(tri.vertexA);
		m_vertices.emplace_back(tri.vertexB);
		m_vertices.emplace_back(tri.vertexC);

		position = tri.GetCenter();
	}

	vec2* NavigationMeshNode::GetVertex(const size_t index)
	{
		if(index < m_vertices.size())
		{
			return &m_vertices[index];
		}

		return nullptr;
	}
}
