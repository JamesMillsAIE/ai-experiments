#include "AiGame.h"

#include <Core/Input.h>
#include <Core/Rendering/Renderer2D.h>
#include <Core/Rendering/Texture.h>

#include "Agents/Agent.h"
#include "Agents/AgentManager.h"
#include "Agents/NameGenerator.h"

#include "AI/AiBrain.h"
#include "AI/Pathfinding/AStar.h"
#include "AI/Pathfinding/NavMesh/NavMesh.h"

#include "Core/Window.h"

using Debugging::Debugger;

using Pathfinding::PathfindingNode;

AiGame::AiGame()
	: m_agentManager{ nullptr }, m_navMesh{ nullptr }, m_aStar{ nullptr }, m_background{ nullptr },
	m_hasDoneFirstClick{ false }, m_firstClickPos{ vec2(0) }
{
}

AiGame::~AiGame() = default;

bool AiGame::Startup()
{
	NameGenerator::Load();

	m_aStar = new AStar;

	m_aStar->InitialiseFrom(WORLD_CELL_SIZE * WORLD_SCALE_FACTOR, "resources/textures/obstacle_map.png", true);

	m_navMesh = new NavMesh(static_cast<float>(m_window->GetWidth()), static_cast<float>(m_window->GetHeight()));

	m_navMesh->AddRandomObstacles(m_random, 12, { 60, 60 }, 10.f);

	m_navMesh->Build();

	m_background = new Texture("resources/textures/world_map.png");

	m_agentManager = new AgentManager(m_random, m_window);

	m_agentManager->Spawn(new Agent);

	return true;
}

void AiGame::Shutdown()
{
	delete m_agentManager;
	m_agentManager = nullptr;

	delete m_background;
	delete m_navMesh;

	delete m_aStar;
}

void AiGame::Tick()
{
	m_agentManager->Tick();

	if (Input* input = Input::Get())
	{
		if (input->WasMouseButtonPressed(MouseButtonLeft))
		{
			if (!m_hasDoneFirstClick)
			{
				m_hasDoneFirstClick = true;
				m_firstClickPos = input->GetMousePos();
			}
			else
			{
				if (m_navMesh->FindPath(input->GetMousePos(), m_firstClickPos, m_path))
				{
					std::cout << "found path!\n";
				}
				else
				{
					std::cout << "oh noes!\n";
				}
			}
		}
	}
}

void AiGame::Render()
{
	if (Renderer2D* renderer = Renderer2D::Get())
	{
		if (!Debugger::InDebugMode())
		{
			renderer->SetRenderColour(1.f, 1.f, 1.f);

			const vec2 backgroundSize =
			{
				static_cast<float>(m_background->GetWidth()) * WORLD_SCALE_FACTOR,
				static_cast<float>(m_background->GetHeight()) * WORLD_SCALE_FACTOR,
			};

			renderer->DrawSprite(m_background, { 0, 0 }, backgroundSize, 0.f, 0.f, 0.f, 0.f);
		}
		else
		{
			if (!m_path.empty())
			{
				renderer->SetRenderColour(1.f, 1.f, 1.f);

				for (size_t i = 1; i < m_path.size(); ++i)
				{
					renderer->DrawLine(m_path[i], m_path[i - 1]);
				}
			}
		}
	}

	m_agentManager->Render();
}