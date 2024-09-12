#include "NavMeshObstacle.h"

#include "Core/Structures/Rectangle.h"

namespace Pathfinding
{
	NavMeshObstacle::NavMeshObstacle(const vec2 pos, const vec2 size, const float padding)
		: position{ pos }, size{ size }, padding{ padding }
	{
	}

	bool NavMeshObstacle::Overlaps(const vec2 otherPos, const vec2 otherSize, const float otherPadding) const
	{
		Rectangle rect = Rectangle(position - vec2(padding), size + vec2(padding * 2.f));
		Rectangle oRect = Rectangle(otherPos - vec2(otherPadding), otherSize + vec2(otherPadding * 2.f));

		return rect.Overlaps(oRect);
	}
}
