#pragma once

#include <list>

#include <Core/Debugging/Debugger.h>

using std::list;
using std::pair;

using Debugging::EVerbosity;

class Behaviour;
class Random;
class Window;
class Renderer2D;
class Agent;

class AiBrain
{
	friend class Agent;

public:
	explicit AiBrain(Agent* agent);
	~AiBrain();

public:
	void Run(Behaviour* behaviour);
	void Stop(Behaviour* behaviour);

	Agent* GetAgent() const;

private:
	typedef void(AiBrain::* ListChange)(Behaviour*);

private:
	list<pair<ListChange, Behaviour*>> m_changes;
	list<Behaviour*> m_behaviours;

	Random* m_random;
	Window* m_window;

	Agent* m_agent;

private:
	void Tick();
	void Render() const;

	void RenderDebug(Renderer2D* renderer, EVerbosity verbosity) const;
	void HandleImGui(EVerbosity verbosity) const;

	void AddBehaviour(Behaviour* behaviour);
	void RemoveBehaviour(Behaviour* behaviour);

};