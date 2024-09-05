#include "Agent.h"

#include <glm/detail/func_geometric.inl>

#include "Core/Renderer2D.h"

Agent::Agent()
	: m_position{ 0 }, m_scale{ 25 }, m_rotation{ 0 }, m_random{ nullptr }
{
}

Agent::~Agent() = default;

void Agent::Tick()
{
}

void Agent::Render()
{
	Renderer2D::Get()->SetRenderColour(1, 0, 0);
	Renderer2D::Get()->DrawCircle(m_position.x, m_position.y, glm::length(m_scale));
}
