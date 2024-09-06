#include "AgentManager.h"

#include <Core/Input.h>
#include <Core/Rendering/Renderer2D.h>
#include <Core/Structures/Random.h>

#include <glm/geometric.hpp>
#include <glm/gtc/round.hpp>

#include "Agent.h"
#include "imgui_internal.h"

#include "Core/Window.h"

void AgentManager::Spawn(Agent* agent)
{
	m_changes.emplace_back(&AgentManager::AddAgent, agent);
}

void AgentManager::Destroy(Agent* agent)
{
	m_changes.emplace_back(&AgentManager::RemoveAgent, agent);
}

AgentManager::AgentManager(Random* random, Window* window)
	: m_random{ random }, m_window{ window }, m_selected{ nullptr }
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

	InitialiseAgent(agent);

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

void AgentManager::InitialiseAgent(Agent* agent)
{
	agent->Initialise(
		glm::roundMultiple(
			vec2
			{
				m_random->Range(0.f, static_cast<float>(m_window->GetWidth())),
				m_random->Range(0.f, static_cast<float>(m_window->GetHeight()))
			},
			vec2(24.f)
		),
		m_random,
		this
	);
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
		renderer->DrawCircleLines(m_selected->GetPosition(), 12.f, 2.f);

		m_selected->RenderDebug(renderer, verbosity);
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
				if (glm::distance(agent->GetPosition(), vec2(input->GetMousePos())) < 12)
				{
					m_selected = agent;

					break;
				}
			}
		}
	}

	if(m_selected)
	{
		m_selected->HandleImGui(verbosity);
	}
}
