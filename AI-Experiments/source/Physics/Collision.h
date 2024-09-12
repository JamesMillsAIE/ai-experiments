#pragma once

#include <aie/bootstrap/Rectangle.h>

#include <glm/vec2.hpp>

using aie::Rectangle;

using glm::vec2;

namespace Physics
{
	class Collision
	{
	public:
		static bool CheckCollisionLines(const vec2& start1, const vec2& end1, const vec2& start2, const vec2& end2, vec2* collisionPoint);
		static bool CheckCollisionPointTriangle(const vec2& point, const vec2& p1, const vec2& p2, const vec2& p3);

		static bool CheckCollisionPointRec(const vec2& point, const Rectangle& rect);
		static bool CheckCollisionRecs(const Rectangle& a, const Rectangle& b);

	};
}