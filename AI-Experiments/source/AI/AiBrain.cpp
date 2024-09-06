#include "AiBrain.h"

#include "Behaviours/Behaviour.h"

AiBrain::AiBrain(Agent* agent)
	: m_random{ nullptr }, m_window{ nullptr }, m_agent{ agent }
{
}

AiBrain::~AiBrain()
{
	for(const auto& behaviour : m_behaviours)
	{
		delete behaviour;
	}

	m_behaviours.clear();
}

void AiBrain::Run(Behaviour* behaviour)
{
	if(std::ranges::find(m_behaviours, behaviour) != m_behaviours.end())
	{
		return;
	}

	m_changes.emplace_back(&AiBrain::AddBehaviour, behaviour);
}

void AiBrain::Stop(Behaviour* behaviour)
{
	if (std::ranges::find(m_behaviours, behaviour) == m_behaviours.end())
	{
		return;
	}

	m_changes.emplace_back(&AiBrain::RemoveBehaviour, behaviour);
}

Agent* AiBrain::GetAgent() const
{
	return m_agent;
}

void AiBrain::Tick()
{
	for(const auto& [fnc, behaviour] : m_changes)
	{
		std::invoke(fnc, this, behaviour);
	}

	m_changes.clear();

	for(const auto& behaviour : m_behaviours)
	{
		behaviour->Tick();
	}
}

void AiBrain::Render() const
{
	for (const auto& behaviour : m_behaviours)
	{
		behaviour->Render();
	}
}

void AiBrain::RenderDebug(Renderer2D* renderer, const EVerbosity verbosity) const
{
	for (const auto& behaviour : m_behaviours)
	{
		behaviour->RenderDebug(renderer, verbosity);
	}
}

void AiBrain::HandleImGui(const EVerbosity verbosity) const
{
	for(const auto& behaviour : m_behaviours)
	{
		behaviour->HandleImGui(verbosity);
	}
}

void AiBrain::AddBehaviour(Behaviour* behaviour)
{
	behaviour->m_owner = this;
	m_behaviours.emplace_back(behaviour);
}

void AiBrain::RemoveBehaviour(Behaviour* behaviour)
{
	m_behaviours.remove(behaviour);
}
