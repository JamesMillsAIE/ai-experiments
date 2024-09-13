#include "HullGenerator.h"

#include <glm/geometric.hpp>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/compatibility.hpp>

using scalar = glm::vec<1, float>;

namespace Pathfinding
{
	vec2 HullGenerator::m_p0;

	void HullGenerator::GenerateHull(vector<vec2>& points)
	{
		float yMin = points[0].y;

		size_t min = 0;

		for(size_t i = 1; i < points.size(); ++i)
		{
			float y = points[i].y;

			if(y < yMin || (all(equal(scalar(yMin), scalar(y))) && points[i].x < points[min].x))
			{
				yMin = points[i].y;
				min = i;
			}
		}

		Swap(points[0], points[min]);

		m_p0 = points[0];
		std::qsort(&points[1], points.size() - 1, sizeof(vec2), Compare);

		size_t m = 1;
		for(size_t i = 0; i < points.size(); ++i)
		{
			while(i < points.size() - 1 && Orientation(m_p0, points[i], points[i + 1]) == 0)
			{
				i++;
			}

			points[m] = points[i];
			m++;
		}

		if(m < 3)
		{
			return;
		}

		stack<vec2> s;
		s.push(points[0]);
		s.push(points[1]);
		s.push(points[2]);

		for(size_t i = 3; i < m; ++i)
		{
			while(s.size() > 1 && Orientation(NextToTop(s), s.top(), points[i]) != 2)
			{
				s.pop();
			}

			s.push(points[i]);
		}

		points.clear();
		while(!s.empty())
		{
			points.emplace_back(s.top());

			s.pop();
		}
	}

	void HullGenerator::AddPaddingToHull(vector<vec2>& hull, const float padding)
	{
		vector<vec2> newHull;
		
		vec2 normA = normalize(hull.back() - hull.front());
		vec2 normB = normalize(hull[1] - hull.front());

		vec2 normal = normalize(lerp(normA, normB, .5f));
		newHull.emplace_back(hull.front() + normal * padding);

		for(size_t i = 1; i < hull.size() - 1; ++i)
		{
			normA = normalize(hull[i - 1] - hull[i]);
			normB = normalize(hull[i + 1] - hull[i]);

			normal = normalize(lerp(normA, normB, .5f));

			newHull.emplace_back(hull[i] + normal * padding);
		}

		hull.clear();
		hull = newHull;
	}

	int HullGenerator::Compare(const void* vp1, const void* vp2)
	{
		const vec2* p1 = static_cast<const vec2*>(vp1);
		const vec2* p2 = static_cast<const vec2*>(vp2);

		const int o = Orientation(m_p0, *p1, *p2);

		if(o == 0)
		{
			return distance(m_p0, *p2) >= distance(m_p0, *p1) ? -1 : 1;
		}

		return o == 2 ? -1 : 1;
	}

	int HullGenerator::Orientation(const vec2 p, const vec2 q, const vec2 r)
	{
		const int val = static_cast<int>(q.y - p.y) * static_cast<int>(r.x - q.x) -
		                static_cast<int>(q.x - p.x) * static_cast<int>(r.y - q.y);

		if (val == 0)
		{
			return 0; // collinear
		}

		return val > 0 ? 1 : 2; // clock or counter-clock wise
	}

	vec2 HullGenerator::NextToTop(stack<vec2>& s)
	{
		const vec2 p = s.top();
		s.pop();

		const vec2 res = s.top();
		s.push(p);

		return res;
	}

	void HullGenerator::Swap(vec2& p1, vec2& p2)
	{
		vec2 temp = p1;
		p1 = p2;
		p2 = temp;
	}
}
