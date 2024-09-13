#include "INavigatable.h"

namespace Pathfinding
{
	INavigatable::~INavigatable() = default;

	float INavigatable::CellSize()
	{
		return 1.f;
	}
}
