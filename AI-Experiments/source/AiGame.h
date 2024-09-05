#pragma once

#include <Core/Game.h>

namespace Pathfinding
{
	class Graph;
}

using Pathfinding::Graph;

class Texture;
class Font;

class AiGame final : public Game
{
public:
	AiGame();
	~AiGame() override;

public:
	bool Startup() override;
	void Shutdown() override;
	void Tick() override;
	void Render() override;

private:
	Graph* m_graph;

	Texture* m_background;

	float m_worldScaleFactor;

};