#include "AiGame.h"

#include <Core/Input.h>
#include <Core/Rendering/Renderer2D.h>
#include <Core/Rendering/Texture.h>

#include "Agents/Agent.h"
#include "Agents/AgentManager.h"
#include "Agents/NameGenerator.h"

#include "AI/Pathfinding/AStar.h"

using Debugging::Debugger;
using Pathfinding::AStar;

constexpr float CELL_SIZE = 16.f;

AiGame::AiGame()
	: m_agentManager{ nullptr }, m_background{ nullptr }, m_worldScaleFactor{ 1.5f }
{
}

AiGame::~AiGame() = default;

bool AiGame::Startup()
{
	NameGenerator::Load();

	AStar::InitialiseFrom(CELL_SIZE * m_worldScaleFactor, "resources/textures/obstacle_map.png", true);

	m_background = new Texture("resources/textures/world_map.png");

	m_agentManager = new AgentManager(m_random);
	m_agentManager->Spawn(new Agent);

	return true;
}

void AiGame::Shutdown()
{
	delete m_agentManager;
	m_agentManager = nullptr;
}

void AiGame::Tick()
{
	m_agentManager->Tick();
}

void AiGame::Render()
{
	if (Renderer2D* renderer = Renderer2D::Get())
	{
		if (!Debugger::IsDebuggingEnabled())
		{
			renderer->SetRenderColour(1.f, 1.f, 1.f);

			const vec2 backgroundSize =
			{
				static_cast<float>(m_background->GetWidth()) * m_worldScaleFactor,
				static_cast<float>(m_background->GetHeight()) * m_worldScaleFactor,
			};

			renderer->DrawSprite(m_background, { 0, 0 }, backgroundSize, 0.f, 0.f, 0.f, 0.f);
		}
	}

	m_agentManager->Render();
}