#pragma once

#include <vector>

#include <glm/vec2.hpp>

#include "Global.h"

#include "Physics/Shapes/Circle.h"
#include "Physics/Shapes/Circle.h"
#include "Physics/Shapes/Line.h"

using std::vector;

using glm::vec2;

using Physics::Line;

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
	public:
		explicit NavigationObstacle(float padding);
		~NavigationObstacle();

	public:
		void AddVertex(vec2 vert);
		void SetPadding(float padding);

		vector<vec2> GetVertices();
		size_t GetNumVertices() const;
		vec2* GetVertex(size_t index);
		float GetPadding() const;

		void Build(EObstacleBuildFlags flags);

		bool Intersects(const NavigationObstacle* other) const;

	private:
		float m_padding;

		vector<vec2> m_vertices;

	private:
		vector<Line> GetEdges() const;

	};
}