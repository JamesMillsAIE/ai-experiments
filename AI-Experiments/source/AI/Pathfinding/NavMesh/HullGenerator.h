#pragma once

#include <glm/vec2.hpp>

#include <stack>
#include <vector>

using glm::vec2;

using std::stack;
using std::vector;

namespace Pathfinding
{
	class HullGenerator
	{
	public:
		static void GenerateHull(vector<vec2>& points);
		static void AddPaddingToHull(vector<vec2>& hull, float padding);

	private:
		static vec2 m_p0;

	private:
		static int Compare(const void* vp1, const void* vp2);
		static int Orientation(vec2 p, vec2 q, vec2 r);
		static vec2 NextToTop(stack<vec2>& s);
		static void Swap(vec2& p1, vec2& p2);

		static vec2 NormalFor(const vec2& target, const vec2& left, const vec2& right);

	};
}
