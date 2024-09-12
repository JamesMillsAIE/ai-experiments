#include "Rectangle.h"

Rectangle::Rectangle(const vec2 position, const vec2 size)
	: position{ position }, size{ size }
{
}

// ReSharper disable CppMemberFunctionMayBeConst
vec2 Rectangle::TopLeft()
{
	return position;
}

vec2 Rectangle::TopRight()
{
	return { position.x + size.x, position.y };
}

vec2 Rectangle::BottomRight()
{
	return position + size;
}

vec2 Rectangle::BottomLeft()
{
	return { position.x, position.y + size.y };
}

bool Rectangle::Overlaps(const Rectangle& other)
{
	return (position.x < (other.position.x + other.size.x) && (position.x + size.x) > other.position.x) &&
		(position.y < (other.position.y + other.size.y) && (position.y + size.y) > other.position.y);
}

// ReSharper restore CppMemberFunctionMayBeConst
