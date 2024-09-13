#include "NavigationObstacle.h"

#include <aie/bootstrap/Renderer.h>

#include <common/shapes.h>

#include "HullGenerator.h"

#include "Physics/Shapes/Line.h"

using Physics::Line;

namespace Pathfinding
{
	NavigationObstacle::NavigationObstacle(const float padding)
		: m_padding{ padding }, m_hasHull{ false }
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
		if (index < m_vertices.size())
		{
			return &m_vertices[index];
		}

		return nullptr;
	}

	vector<vec2> NavigationObstacle::GetHullVertices()
	{
		return m_hullVertices;
	}

	size_t NavigationObstacle::GetNumHullVertices() const
	{
		return m_hullVertices.size();
	}

	vec2* NavigationObstacle::GetHullVertex(size_t index)
	{
		if (index < m_hullVertices.size())
		{
			return &m_hullVertices[index];
		}

		return nullptr;
	}

	vector<vec2> NavigationObstacle::GetPaddedVertices()
	{
		return m_paddedVertices;
	}

	size_t NavigationObstacle::GetNumPaddedVertices() const
	{
		return m_paddedVertices.size();
	}

	vec2* NavigationObstacle::GetPaddedVertex(size_t index)
	{
		if (index < m_paddedVertices.size())
		{
			return &m_paddedVertices[index];
		}

		return nullptr;
	}

	float NavigationObstacle::GetPadding() const
	{
		return m_padding;
	}

	bool NavigationObstacle::IsConvexHull() const
	{
		return m_hasHull;
	}

	void NavigationObstacle::Build(const EObstacleBuildFlags flags)
	{
		vector<vec2> points;
		points.reserve(m_vertices.size());

		for (const auto& vert : m_vertices)
		{
			points.emplace_back(vert);
		}

		if ((flags & EObstacleBuildFlags::GenerateConvexHull) == EObstacleBuildFlags::GenerateConvexHull)
		{
			HullGenerator::GenerateHull(points);
			for (const auto& point : points)
			{
				m_hullVertices.emplace_back(point);
			}

			m_hasHull = true;
		}

		HullGenerator::AddPaddingToHull(points, m_padding);
		for (const auto& point : points)
		{
			m_paddedVertices.emplace_back(point);
		}
	}

	bool NavigationObstacle::Intersects(const NavigationObstacle* other) const
	{
		bool didIntersect = false;

		const vector<Line> myEdges = GetEdges();
		const vector<Line> otherEdges = other->GetEdges();

		for (auto& myEdge : myEdges)
		{
			for (const auto& otherEdge : otherEdges)
			{
				const vec2* intersection = myEdge.DoesIntersect(otherEdge);

				didIntersect |= intersection != nullptr;

				delete intersection;
			}
		}

		return didIntersect;
	}

	void NavigationObstacle::RenderDebuggingTools(Renderer* renderer, const EVerbosity verbosity) const
	{
		vector<vec2> verts = m_hasHull ? m_hullVertices : m_paddedVertices;

		RenderVertexGroup(renderer, verts, { 255, 0, 0, 255 });

		if (verbosity == EVerbosity::Advanced)
		{
			RenderVertexGroup(renderer, m_vertices, { 255, 0, 255, 255 });
		}
	}

	void NavigationObstacle::RenderVertexGroup(Renderer* renderer, vector<vec2> group, const Color color) const
	{
		vec2* a;
		vec2* b;

		for (size_t i = 0; i < group.size() - 1; ++i)
		{
			a = &group[i];
			b = &group[i + 1];

			renderer->DrawLine(
				{ a->x, a->y },
				{ b->x, b->y },
				1.f, color
			);
		}

		a = group.data();
		b = &group[group.size() - 1];

		renderer->DrawLine(
			{ a->x, a->y },
			{ b->x, b->y },
			1.f, color
		);
	}

	vector<Line> NavigationObstacle::GetEdges() const
	{
		vector<vec2> verts = m_hasHull ? m_hullVertices : m_paddedVertices;
		vector<Line> edges;

		edges.emplace_back(verts[0], verts[verts.size() - 1]);

		for (size_t i = 0; i < verts.size() - 1; ++i)
		{
			edges.emplace_back(verts[i], verts[i + 1]);
		}

		return edges;
	}
}
