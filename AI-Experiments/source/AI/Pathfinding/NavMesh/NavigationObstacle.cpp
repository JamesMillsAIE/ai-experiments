#include "NavigationObstacle.h"

#include <common/shapes.h>

#include "HullGenerator.h"

#include "Physics/Shapes/Line.h"

using Physics::Line;

namespace Pathfinding
{
	NavigationObstacle::NavigationObstacle(const float padding)
		: m_padding{ padding }
	{
	}

	NavigationObstacle::~NavigationObstacle()
	{
		m_vertices.clear();
	}

	void NavigationObstacle::AddVertex(const vec2 vert)
	{
		m_vertices.emplace_back(vert);
	}

	void NavigationObstacle::SetPadding(const float padding)
	{
		m_padding = padding;
	}

	vector<vec2> NavigationObstacle::GetVertices()
	{
		return m_vertices;
	}

	size_t NavigationObstacle::GetNumVertices() const
	{
		return m_vertices.size();
	}

	vec2* NavigationObstacle::GetVertex(const size_t index)
	{
		if(index < m_vertices.size())
		{
			return &m_vertices[index];
		}

		return nullptr;
	}

	float NavigationObstacle::GetPadding() const
	{
		return m_padding;
	}

	void NavigationObstacle::Build(const EObstacleBuildFlags flags)
	{
		vector<vec2> points;
		points.reserve(m_vertices.size());

		for(const auto& vert : m_vertices)
		{
			points.emplace_back(vert);
		}

		m_vertices.clear();
		m_vertices = vector<vec2>();

		if ((flags & EObstacleBuildFlags::GenerateConvexHull) == EObstacleBuildFlags::GenerateConvexHull)
		{
			HullGenerator::GenerateHull(points);
		}

		if ((flags & EObstacleBuildFlags::AddPaddingToHull) == EObstacleBuildFlags::AddPaddingToHull)
		{
			HullGenerator::AddPaddingToHull(points, m_padding);
		}

		for(const auto& p : points)
		{
			m_vertices.emplace_back(p);
		}
	}

	bool NavigationObstacle::Intersects(const NavigationObstacle* other) const
	{
		bool didIntersect = false;

		const vector<Line> myEdges = GetEdges();
		const vector<Line> otherEdges = other->GetEdges();

		for(auto& myEdge : myEdges)
		{
			for(const auto& otherEdge : otherEdges)
			{
				const vec2* intersection = myEdge.DoesIntersect(otherEdge);

				didIntersect |= intersection != nullptr;

				delete intersection;
			}
		}

		return didIntersect;
	}

	vector<Line> NavigationObstacle::GetEdges() const
	{
		vector<Line> edges;

		edges.emplace_back(m_vertices[0], m_vertices[m_vertices.size() - 1]);

		for(size_t i = 0; i < m_vertices.size() - 1; ++i)
		{
			edges.emplace_back(m_vertices[i], m_vertices[i + 1]);
		}

		return edges;
	}
}
