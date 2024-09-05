#include "AgentManager.h"

#include "Agent.h"

#include "Core/Random.h"

void AgentManager::Spawn(Agent* agent)
{
	m_changes.emplace_back(&AgentManager::AddAgent, agent);
}

void AgentManager::Destroy(Agent* agent)
{
	m_changes.emplace_back(&AgentManager::RemoveAgent, agent);
}

AgentManager::AgentManager(Random* random)
	: m_random{ random }
{
	
}

AgentManager::~AgentManager()
{
	for(auto& agent : m_agents)
	{
		delete agent;
	}
}

void AgentManager::Tick()
{
	for(auto& [change, agent] : m_changes)
	{
		(this->*change)(agent);
	}

	m_changes.clear();

	for(auto& agent : m_agents)
	{
		agent->Tick();
	}
}

void AgentManager::Render() const
{
	for(auto& agent : m_agents)
	{
		agent->Render();
	}
}

void AgentManager::AddAgent(Agent* agent)
{
	if(std::ranges::find(m_agents, agent) != m_agents.end())
	{
		return;
	}

	agent->m_position =
	{
		m_random->Range(0.f, 1080.f),
		m_random->Range(0.f, 720.f),
	};
	agent->m_random = m_random;
	m_agents.emplace_back(agent);
}

void AgentManager::RemoveAgent(Agent* agent)
{
	if(std::ranges::find(m_agents, agent) == m_agents.end())
	{
		return;
	}

	m_agents.remove(agent);
}