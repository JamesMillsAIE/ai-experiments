#include "Edge.h"

namespace Pathfinding
{
	Edge::Edge(Node* endpoint, const float cost)
		: endpoint{ endpoint }, cost{ cost }
	{
	}

	Edge::~Edge() = default;
}
