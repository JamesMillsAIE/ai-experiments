#pragma once

#include <list>
#include <string>

#include <glm/vec2.hpp>

#include "AgentManager.h"

using std::list;
using std::string;

using glm::vec2;

class Renderer2D;
class Sprite;
class Random;

class Agent
{
public:
	Agent();
	~Agent();

public:
	void Tick();
	void Render();

	void RenderDebug(Renderer2D* renderer, EVerbosity verbosity);
	void HandleImGui(EVerbosity verbosity);
	void Highlight(Renderer2D* renderer);

	vec2 GetPosition() const;

protected:
	vec2 m_position;
	vec2 m_scale;
	float m_rotation;

	Random* m_random;

	AgentManager* m_owner;
	string m_name;

	Sprite* m_sprite;

private:
	friend void AgentManager::InitialiseAgent(Agent* agent);

private:
	void Initialise(vec2 position, Random* random, AgentManager* owner);

};