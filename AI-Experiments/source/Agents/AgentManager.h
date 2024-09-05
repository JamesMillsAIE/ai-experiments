#pragma once

#include <list>

using std::list;
using std::pair;

class Agent;
class Random;

class AgentManager
{
	friend class Game;

public:
	void Spawn(Agent* agent);
	void Destroy(Agent* agent);

private:
	typedef void(AgentManager::* ListChange)(Agent*);

private:
	list<pair<ListChange, Agent*>> m_changes;
	list<Agent*> m_agents;

	Random* m_random;

private:
	AgentManager(Random* random);
	~AgentManager();

private:
	void Tick();
	void Render() const;

	void AddAgent(Agent* agent);
	void RemoveAgent(Agent* agent);

};