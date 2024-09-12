#pragma once

#include <aie/bootstrap/Bootstrap2D.h>
#include <aie/bootstrap/Vector2.h>

namespace aie
{
	class DLL Rectangle
	{
	public:
		Vector2 position;
		Vector2 size;

	public:
		Rectangle();
		Rectangle(float x, float y, float w, float h);
		Rectangle(Vector2 pos, Vector2 size);

	};
}
