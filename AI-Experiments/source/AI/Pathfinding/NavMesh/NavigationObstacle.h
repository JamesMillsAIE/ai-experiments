#pragma once

#include <vector>

#include <aie/bootstrap/Color.h>

#include <glm/vec2.hpp>

#include "Global.h"

#include "Debugging/Debugger.h"

#include "Physics/Shapes/Line.h"

using std::vector;

using glm::vec2;

using Physics::Line;
using Debugging::EVerbosity;

using aie::Color;

namespace p2t
{
	struct Point;
}

using p2t::Point;

namespace Pathfinding
{
	enum class EObstacleBuildFlags
	{
		None = -1,
		GenerateConvexHull = 0x01 << 0,
		AddPaddingToHull = 0x01 << 1
	};

	ENUM_FLAG_OPERATORS(EObstacleBuildFlags);

	class NavigationObstacle
	{
		friend class NavigationMesh;

	public:
		explicit NavigationObstacle(float padding);
		~NavigationObstacle();

	public:
		void AddVertex(vec2 vert);
		void SetPadding(float padding);

		vector<vec2> GetVertices();
		size_t GetNumVertices() const;
		vec2* GetVertex(size_t index);

		vector<vec2> GetHullVertices();
		size_t GetNumHullVertices() const;
		vec2* GetHullVertex(size_t index);

		vector<vec2> GetPaddedVertices();
		size_t GetNumPaddedVertices() const;
		vec2* GetPaddedVertex(size_t index);

		float GetPadding() const;
		bool IsConvexHull() const;

		void Build(EObstacleBuildFlags flags);

		bool Intersects(const NavigationObstacle* other) const;

	private:
		float m_padding;
		bool m_hasHull;

		vector<vec2> m_vertices;
		vector<vec2> m_hullVertices;
		vector<vec2> m_paddedVertices;

	private:
		vector<Line> GetEdges() const;

		void RenderDebuggingTools(Renderer* renderer, EVerbosity verbosity) const;
		void RenderVertexGroup(Renderer* renderer, vector<vec2> group, Color color) const;

	};
}