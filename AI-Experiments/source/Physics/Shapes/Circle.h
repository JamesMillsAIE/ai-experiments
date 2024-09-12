#pragma once

#include <glm/vec2.hpp>

using glm::vec2;

namespace Physics
{
	class Circle
	{
	public:
		vec2 position;
		float radius;

	public:
		Circle(const vec2& position, float radius);

	};
}
