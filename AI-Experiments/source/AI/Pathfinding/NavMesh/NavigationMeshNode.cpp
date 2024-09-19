#include "NavigationMeshNode.h"

#include <glm/ext/vector_relational.hpp>

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

	size_t NavigationMeshNode::GetAdjacentVertices(const NavigationMeshNode* other, vec2* adjacent) const
	{
		size_t count = 0;

		for(auto& v : m_vertices)
		{
			for(auto& otherV : other->m_vertices)
			{
				if(all(equal(v, otherV)))
				{
					adjacent[count++] = v;
					break;
				}
			}
		}

		return count;
	}
}
