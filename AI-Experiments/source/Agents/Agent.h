#pragma once

#include <glm/vec2.hpp>

using glm::vec2;

class Random;

class Agent
{
	friend class AgentManager;

public:
	Agent();
	~Agent();

public:
	void Tick(float deltaTime);
	void Render();

private:
	vec2 m_position;
	vec2 m_scale;
	float m_rotation;

	Random* m_random;

};