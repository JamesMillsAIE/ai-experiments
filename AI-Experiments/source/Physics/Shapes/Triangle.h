#pragma once

#include <glm/vec2.hpp>

using glm::vec2;

namespace Physics
{
	class Triangle
	{
	public:
		vec2 vertexA;
		vec2 vertexB;
		vec2 vertexC;

	public:
		Triangle(const vec2& a, const vec2& b, const vec2& c);

	public:
		vec2 GetCenter() const;
		float Area() const;

	};
}
