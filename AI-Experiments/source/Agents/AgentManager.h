#pragma once

#include <list>

#include <Core/Debugging/IDebugHandler.h>

using std::list;
using std::pair;

class Agent;
class Random;
class Window;

using Debugging::IDebugHandler;
using Debugging::EVerbosity;

class AgentManager final : public IDebugHandler
{
	friend class AiGame;
	friend class Agent;

public:
	void Spawn(Agent* agent);
	void Destroy(Agent* agent);

protected:
	string DebugCategory() override;
	void RenderDebuggingTools(Renderer2D* renderer, EVerbosity verbosity) override;
	void HandleImGui(EVerbosity verbosity) override;

private:
	typedef void(AgentManager::* ListChange)(Agent*);

private:
	list<pair<ListChange, Agent*>> m_changes;
	list<Agent*> m_agents;

	Random* m_random;
	Window* m_window;

	Agent* m_selected;

private:
	AgentManager(Random* random, Window* window);
	~AgentManager() override;

private:
	void Tick();
	void Render() const;

	void AddAgent(Agent* agent);
	void RemoveAgent(Agent* agent);

	void InitialiseAgent(Agent* agent);

};