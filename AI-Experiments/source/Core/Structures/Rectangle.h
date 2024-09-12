#pragma once

#include <glm/vec2.hpp>

using glm::vec2;

class Rectangle
{
public:
	vec2 position;
	vec2 size;

public:
	Rectangle(vec2 position, vec2 size);

public:
	vec2 TopLeft();
	vec2 TopRight();
	vec2 BottomRight();
	vec2 BottomLeft();

	bool Overlaps(const Rectangle& other);

};