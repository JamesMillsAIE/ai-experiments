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
class AiBrain;

class Agent
{
public:
	Agent();
	~Agent();

public:
	void Tick() const;
	void Render() const;

	void RenderDebug(Renderer2D* renderer, EVerbosity verbosity) const;
	void HandleImGui(EVerbosity verbosity) const;
	void Highlight(Renderer2D* renderer) const;

	vec2 GetPosition() const;

	AiBrain* GetBrain() const;

protected:
	vec2 m_position;
	vec2 m_scale;
	float m_rotation;

	Random* m_random;

	AgentManager* m_owner;
	string m_name;

	Sprite* m_sprite;
	AiBrain* m_brain;

private:
	friend void AgentManager::InitialiseAgent(Agent* agent);

private:
	void Initialise(vec2 position, Random* random, AgentManager* owner);

};