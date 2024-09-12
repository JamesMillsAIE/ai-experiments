#pragma once

#include <glm/vec2.hpp>

using glm::vec2;

namespace Physics
{
	class Line
	{
	public:
		vec2 start;
		vec2 end;

	public:
		Line();
		Line(vec2 start, vec2 end);

	public:
		bool HasStart() const;
		bool HasEnd() const;

		vec2 GetMiddle() const;
		vec2 GetClosestPoint(vec2 point);
		vec2* DoesIntersect(Line line) const;

	};
}
