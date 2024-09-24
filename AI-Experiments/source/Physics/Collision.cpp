#include "Collision.h"

#include <corecrt_math.h>

#include "Shapes/Circle.h"
#include "Shapes/Line.h"
#include "Shapes/Triangle.h"

namespace Physics
{
	bool Collision::CheckCollisionPointTriangle(const vec2& point, const Triangle& tri)
	{
		bool collision = false;

		const float alpha = ((tri.vertexB.y - tri.vertexC.y) * (point.x - tri.vertexC.x) + (tri.vertexC.x - tri.vertexB.x) * (point.y - tri.vertexC.y)) /
			((tri.vertexB.y - tri.vertexC.y) * (tri.vertexA.x - tri.vertexC.x) + (tri.vertexC.x - tri.vertexB.x) * (tri.vertexA.y - tri.vertexC.y));

		const float beta = ((tri.vertexC.y - tri.vertexA.y) * (point.x - tri.vertexC.x) + (tri.vertexA.x - tri.vertexC.x) * (point.y - tri.vertexC.y)) /
			((tri.vertexB.y - tri.vertexC.y) * (tri.vertexA.x - tri.vertexC.x) + (tri.vertexC.x - tri.vertexB.x) * (tri.vertexA.y - tri.vertexC.y));

		const float gamma = 1.0f - alpha - beta;

		if (alpha > 0 && beta > 0 && gamma > 0)
		{
			collision = true;
		}

		return collision;
	}

	bool Collision::CheckCollisionPointRec(const vec2& point, const Rectangle& rect)
	{
		bool collision = false;

		if (point.x >= rect.position.x && point.x < rect.position.x + rect.size.x &&
			point.y >= rect.position.y && point.y < rect.position.y + rect.size.y)
		{
			collision = true;
		}

		return collision;
	}

	bool Collision::CheckCollisionPointCircle(const vec2& point, const Circle& circle)
	{
		bool collision = false;

		float distanceSquared = (point.x - circle.position.x) * (point.x - circle.position.x) + 
			(point.y - circle.position.y) * (point.y - circle.position.y);

		if (distanceSquared <= circle.radius * circle.radius)
		{
			collision = true;
		}

		return collision;
	}

	bool Collision::CheckCollisionLines(const Line& a, const Line& b, vec2* collisionPoint)
	{
		bool collision = false;

		float div = (b.end.y - b.start.y) * (a.end.x - a.start.x) - (b.end.x - b.start.x) * (a.end.y - a.start.y);

		if (fabsf(div) >= FLT_EPSILON)
		{
			collision = true;

			float xi = ((b.start.x - b.end.x) * (a.start.x * a.end.y - a.start.y * a.end.x) - (a.start.x - a.end.x) * (b.start.x * b.end.y - b.start.y * b.end.x)) / div;
			float yi = ((b.start.y - b.end.y) * (a.start.x * a.end.y - a.start.y * a.end.x) - (a.start.y - a.end.y) * (b.start.x * b.end.y - b.start.y * b.end.x)) / div;

			if ((fabsf(a.start.x - a.end.x) > FLT_EPSILON && (xi < fminf(a.start.x, a.end.x) || xi > fmaxf(a.start.x, a.end.x))) ||
				(fabsf(b.start.x - b.end.x) > FLT_EPSILON && (xi < fminf(b.start.x, b.end.x) || xi > fmaxf(b.start.x, b.end.x))) ||
				(fabsf(a.start.y - a.end.y) > FLT_EPSILON && (yi < fminf(a.start.y, a.end.y) || yi > fmaxf(a.start.y, a.end.y))) ||
				(fabsf(b.start.y - b.end.y) > FLT_EPSILON && (yi < fminf(b.start.y, b.end.y) || yi > fmaxf(b.start.y, b.end.y))))
			{
				collision = false;
			}

			if (collision && collisionPoint != nullptr)
			{
				collisionPoint->x = xi;
				collisionPoint->y = yi;
			}
		}

		return collision;
	}

	bool Collision::CheckCollisionRecs(const Rectangle& a, const Rectangle& b)
	{
		bool collision = false;

		if (a.position.x < b.position.x + b.size.x && a.position.x + a.size.x > b.position.x &&
			(a.position.y < b.position.y + b.size.y && a.position.y + a.size.y > b.position.y))
		{
			collision = true;
		}

		return collision;
	}

	bool Collision::CheckCollisionCircles(const Circle& a, const Circle& b)
	{
		const float dx = b.position.x - a.position.x;      // X distance between centers
		const float dy = b.position.y - a.position.y;      // Y distance between centers

		const float distanceSquared = dx * dx + dy * dy; // Distance between centers squared
		const float radiusSum = a.radius + b.radius;

		const bool collision = distanceSquared <= (radiusSum * radiusSum);

		return collision;
	}
}
