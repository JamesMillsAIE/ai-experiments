#include "AgentManager.h"

#include <glm/geometric.hpp>
#include <glm/gtc/round.hpp>

#include "Agent.h"

#include "Core/Input.h"
#include "Core/Random.h"
#include "Core/Renderer2D.h"

void AgentManager::Spawn(Agent* agent)
{
	m_changes.emplace_back(&AgentManager::AddAgent, agent);
}

void AgentManager::Destroy(Agent* agent)
{
	m_changes.emplace_back(&AgentManager::RemoveAgent, agent);
}

AgentManager::AgentManager(Random* random)
	: m_random{ random }, m_selected{ nullptr }
{

}

AgentManager::~AgentManager()
{
	for (auto& agent : m_agents)
	{
		delete agent;
	}
}

void AgentManager::Tick()
{
	for (auto& [change, agent] : m_changes)
	{
		(this->*change)(agent);
	}

	m_changes.clear();

	for (auto& agent : m_agents)
	{
		agent->Tick();
	}
}

void AgentManager::Render() const
{
	for (auto& agent : m_agents)
	{
		agent->Render();
	}
}

void AgentManager::AddAgent(Agent* agent)
{
	if (std::ranges::find(m_agents, agent) != m_agents.end())
	{
		return;
	}

	agent->Initialise(glm::roundMultiple(
		{
			m_random->Range(0.f, 912.f),
			m_random->Range(0.f, 840.f),
		},
		vec2(12.f)), 
		m_random, 
		this
	);

	m_agents.emplace_back(agent);
}

void AgentManager::RemoveAgent(Agent* agent)
{
	if (std::ranges::find(m_agents, agent) == m_agents.end())
	{
		return;
	}

	m_agents.remove(agent);
}

string AgentManager::DebugCategory()
{
	return "Agents";
}

void AgentManager::RenderDebuggingTools(Renderer2D* renderer, EVerbosity verbosity)
{
	if(m_selected)
	{
		renderer->SetRenderColour(vec3(0));
		renderer->DrawCircleLines(m_selected->m_position, 12.f, 2.f);

		m_selected->RenderDebug(renderer);
	}
}

void AgentManager::HandleImGui(EVerbosity verbosity)
{
	if (Input* input = Input::Get())
	{
		if (input->WasMouseButtonPressed(MouseButtonLeft))
		{
			m_selected = nullptr;

			for (auto& agent : m_agents)
			{
				if (glm::distance(agent->m_position, vec2(input->GetMousePos())) < 12)
				{
					m_selected = agent;

					break;
				}
			}
		}
	}
}
