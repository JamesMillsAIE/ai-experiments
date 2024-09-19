#pragma once

#include <vector>

#include <glm/vec2.hpp>

#include "Debugging/IDebugHandler.h"

using std::vector;

using glm::vec2;

using Debugging::IDebugHandler;

namespace aie
{
	class Random;
}

using aie::Random;

namespace Pathfinding
{
	class Node;

	class INavigatable : public IDebugHandler
	{
	public:
		~INavigatable() override;

	public:
		virtual float CellSize();

		virtual Node* ClosestNode(vec2 worldPos) = 0;
		virtual Node* RandomNode(Random* random) = 0;
		virtual vector<Node*> GetNodes() = 0;

		virtual vector<vec2> SmoothPath(const vector<Node*>& path, vec2* start = nullptr, vec2* end = nullptr) = 0;

	};
}
