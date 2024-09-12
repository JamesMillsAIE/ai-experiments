#include "Triangle.h"

namespace Physics
{
	Triangle::Triangle(const vec2& a, const vec2& b, const vec2& c)
		: vertexA{ a }, vertexB{ b }, vertexC{ c }
	{

	}

	vec2 Triangle::GetCenter() const
	{
		vec2 center = vertexA + vertexB + vertexC;

		center /= 3;

		return center;
	}
}
