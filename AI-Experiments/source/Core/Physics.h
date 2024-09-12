#pragma once

#include <glm/vec2.hpp>

using glm::vec2;

class Physics
{
public:
	static bool CheckCollisionLines(vec2 start1, vec2 end1, vec2 start2, vec2 end2, vec2* collisionPoint);
	static bool CheckCollisionPointTriangle(vec2 point, vec2 p1, vec2 p2, vec2 p3);

};