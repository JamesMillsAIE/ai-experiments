#pragma once

namespace Pathfinding
{
	class PathfindingNode;

	class PathfindingEdge
	{
	public:
		PathfindingNode* endPoint;
		float cost;

	public:
		PathfindingEdge(PathfindingNode* endPoint, float cost);

	};
}
