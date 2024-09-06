#include "Agent.h"

#include <Core/Rendering/Renderer2D.h>

#include <glm/detail/func_geometric.inl>

#include "AgentManager.h"
#include "NameGenerator.h"

Agent::Agent()
	: m_position{ 0 }, m_scale{ 12.f, 0.f }, m_rotation{ 0 }, m_random{ nullptr }, m_owner{ nullptr }
{
}

Agent::~Agent() = default;

void Agent::Tick()
{
	
}

void Agent::Render()
{
	Renderer2D::Get()->SetRenderColour(1, 0, 0);
	Renderer2D::Get()->DrawCircle(m_position, glm::length(m_scale));
}

void Agent::Initialise(vec2 position, Random* random, AgentManager* owner)
{
	m_position = position;
	m_random = random;
	m_owner = owner;

	m_name = NameGenerator::NewName(random);
}

void Agent::RenderDebug(Renderer2D* renderer)
{
	
}
