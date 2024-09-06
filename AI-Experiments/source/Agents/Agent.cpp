#include "Agent.h"

#include <Core/Rendering/Renderer2D.h>
#include <Core/Rendering/Texture.h>

#include <glm/detail/func_geometric.inl>

#include "imgui_internal.h"
#include "NameGenerator.h"

#include "Core/Rendering/Sprite.h"

Agent::Agent()
	: m_position{ 0 }, m_scale{ WORLD_CELL_SIZE, WORLD_CELL_SIZE }, m_rotation{ 0 }, m_random{ nullptr },
	m_owner{ nullptr }, m_sprite{ nullptr }
{
}

Agent::~Agent() = default;

void Agent::Tick()
{

}

void Agent::Render()
{
	if(m_sprite)
	{
		m_sprite->Draw(m_position, m_scale * WORLD_SCALE_FACTOR);
	}
}

void Agent::Initialise(vec2 position, Random* random, AgentManager* owner)
{
	m_position = position;
	m_random = random;
	m_owner = owner;

	m_name = NameGenerator::NewName(random);

	m_sprite = new Sprite("resources/textures/characters.png");
	m_sprite->SetUV(0, 104.f, WORLD_CELL_SIZE, WORLD_CELL_SIZE);
}

void Agent::RenderDebug(Renderer2D* renderer, EVerbosity verbosity)
{
	renderer->SetRenderColour(1.f, 0.f, 0.f, 1.f);
	renderer->DrawBox(m_position, m_scale);
}

void Agent::HandleImGui(EVerbosity verbosity)
{
	ImGui::Text("Name: %s", m_name.c_str());
}

void Agent::Highlight(Renderer2D* renderer)
{
	renderer->SetRenderColour(1.f, 1.f, 1.f, 1.f);
	renderer->DrawBoxLines(m_position, m_scale, 0.f, 2.f);
}

vec2 Agent::GetPosition() const
{
	return m_position;
}
