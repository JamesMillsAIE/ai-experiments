#pragma once

#include <aie/bootstrap/Rectangle.h>

#include <glm/vec2.hpp>

using aie::Rectangle;

using glm::vec2;

namespace Physics
{
	class Circle;
	class Line;
	class Triangle;

	class Collision
	{
	public:
		static bool CheckCollisionPointTriangle(const vec2& point, const Triangle& tri);
		static bool CheckCollisionPointRec(const vec2& point, const Rectangle& rect);
		static bool CheckCollisionPointCircle(const vec2& point, const Circle& circle);

		static bool CheckCollisionLines(const Line& a, const Line& b, vec2* collisionPoint);
		static bool CheckCollisionRecs(const Rectangle& a, const Rectangle& b);
		static bool CheckCollisionCircles(const Circle& a, const Circle& b);

	};
}