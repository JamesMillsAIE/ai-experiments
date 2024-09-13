#pragma once

#include "AI/Pathfinding/Node.h"

#include <vector>

using std::vector;

namespace Physics
{
	class Triangle;
}

using Physics::Triangle;

namespace Pathfinding
{
	class NavigationMeshNode final : public Node
	{
	public:
		NavigationMeshNode();

	public:
		void AddVertices(Triangle& tri);

		vec2* GetVertex(size_t index);

	private:
		vector<vec2> m_vertices;

	};
}
