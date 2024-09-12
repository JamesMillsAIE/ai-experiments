#pragma once

#include <aie/bootstrap/Bootstrap2D.h>

namespace aie
{
	class DLL Color
	{
	public:
		unsigned char r;
		unsigned char g;
		unsigned char b;
		unsigned char a;

	public:
		Color();
		Color(unsigned char r, unsigned char g, unsigned char b, unsigned char a);

	public:
		bool operator==(const Color& rhs) const;
		bool operator!=(const Color& rhs) const;

	};
}