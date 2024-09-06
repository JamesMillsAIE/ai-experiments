#pragma once

#include <list>
#include <string>

#include <glm/vec2.hpp>

using std::list;
using std::string;

using glm::vec2;

class Renderer2D;
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
	string m_name;

private:
	void Initialise(vec2 position, Random* random, AgentManager* owner);

	void RenderDebug(Renderer2D* renderer);

};