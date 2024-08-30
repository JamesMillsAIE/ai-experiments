#pragma once

#include <Core/Game.h>

namespace Pathfinding
{
	class Graph;
}

using Pathfinding::Graph;

class AiGame final : public Game
{
public:
	AiGame();
	~AiGame() override;

public:
	bool Startup() override;
	void Shutdown() override;
	void Tick(float deltaTime) override;
	void Render() override;

private:
	Graph* m_graph;

};