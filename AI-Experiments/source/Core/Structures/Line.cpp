#include "Line.h"

#include <Core/Physics.h>

#include <glm/geometric.hpp>

Line::Line()
	: start{ 0 }, end{ 0, 1 }
{
}

Line::Line(const vec2 start, const vec2 end)
	: start{ start }, end{ end }
{
}

bool Line::HasStart() const
{
	return start != vec2(0);
}

bool Line::HasEnd() const
{
	return end != vec2(0);
}

vec2 Line::GetMiddle() const
{
	return { (start.x + end.x) / 2, (start.y + end.y) / 2 };
}

vec2 Line::GetClosestPoint(const vec2 point)
{
	const float middleDist = distance(point, GetMiddle());
	const float startDist = distance(point, { start.x, start.y });
	const float endDist = distance(point, { end.x, end.y });

	if (startDist < endDist)
	{
		if (startDist < middleDist)
		{
			return { start.x, start.y };
		}

		return GetMiddle();
	}

	if (middleDist < endDist)
	{
		return GetMiddle();
	}

	return { end.x, end.y };
}

vec2* Line::DoesIntersect(const Line line) const
{
	vec2* intersection = new vec2;

	if(Physics::CheckCollisionLines(start, end, line.start, line.end, intersection))
	{
		return intersection;
	}
	else
	{
		delete intersection;
		return nullptr;
	}
}
