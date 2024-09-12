#pragma once

#include <list>

#include <Core/Debugging/IDebugHandler.h>

class Random;

using Debugging::IDebugHandler;

namespace Pathfinding
{
	class PathfindingNode;

	class IPathProvider : public IDebugHandler
	{
	public:
		IPathProvider() = default;

	public:
		virtual PathfindingNode* RandomNode(const Random* random) const = 0;
		virtual PathfindingNode* GetClosestNode(const vec2& position) const = 0;

		virtual bool FindPath(const vec2& start, const vec2& end, vector<vec2>& path) = 0;

	};
}
