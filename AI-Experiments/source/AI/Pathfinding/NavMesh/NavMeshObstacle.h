#pragma once

#include <glm/vec2.hpp>

using glm::vec2;

namespace Pathfinding
{
	class NavMeshObstacle
	{
	public:
		vec2 position;
		vec2 size;
		float padding;

	public:
		NavMeshObstacle(vec2 pos, vec2 size, float padding);

	public:
		bool Overlaps(vec2 otherPos, vec2 otherSize, float otherPadding) const;

	};
}