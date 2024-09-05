#pragma once

#include <list>

#include <glm/vec2.hpp>

class Renderer2D;
using std::list;

using glm::vec2;

namespace Pathfinding
{
	class Node;
}

using Pathfinding::Node;

class Random;
class AgentManager;

class Agent
{
	friend class AgentManager;

public:
	Agent();
	~Agent();

public:
	void Tick();
	void Render();

private:
	vec2 m_position;
	vec2 m_scale;
	float m_rotation;

	Random* m_random;

	AgentManager* m_owner;

private:
	void Initialise(vec2 position, Random* random, AgentManager* owner);

	void RenderDebug(Renderer2D* renderer);

};