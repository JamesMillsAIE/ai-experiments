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

	float Triangle::Area() const
	{
		const float ax = vertexB[0] - vertexA[0];
		const float ay = vertexB[1] - vertexA[1];
		const float bx = vertexC[0] - vertexA[0];
		const float by = vertexC[1] - vertexA[1];
		return bx * ay - ax * by;
	}
}
