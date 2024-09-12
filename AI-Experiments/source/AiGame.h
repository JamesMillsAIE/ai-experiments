#pragma once

#include <Core/Game.h>

#include <vector>

#include <glm/vec2.hpp>

using std::vector;

using glm::vec2;

namespace Pathfinding
{
	class NavMesh;
	class AStar;
	class PathfindingNode;
}

using Pathfinding::NavMesh;
using Pathfinding::AStar;
using Pathfinding::PathfindingNode;

class Texture;
class Font;
class AgentManager;

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
	AgentManager* m_agentManager;
	NavMesh* m_navMesh;
	AStar* m_aStar;

	Texture* m_background;

	bool m_hasDoneFirstClick;
	vec2 m_firstClickPos;
	vector<vec2> m_path;

};