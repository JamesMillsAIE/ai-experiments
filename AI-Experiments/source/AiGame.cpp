#include "AiGame.h"

#include <imgui/imgui.h>

#include "AI/Locomotion/AStar.h"

#include "Core/Input.h"
#include "Core/Renderer2D.h"
#include "Core/Texture.h"
#include "Core/Window.h"

using Debugging::Debugger;

constexpr float CELL_SIZE = 16.f;

AiGame::AiGame()
	: m_graph{ nullptr }, m_background{ nullptr }, m_worldScaleFactor{ 1.5f }
{
}

AiGame::~AiGame()
{
	delete m_graph;
}

bool AiGame::Startup()
{
	const Texture* obstacleMap = new Texture("resources/textures/obstacle_map.png");

	const float spacing = CELL_SIZE * m_worldScaleFactor;

	m_graph = new Graph(obstacleMap->GetWidth(), obstacleMap->GetHeight(), spacing);
	m_graph->BuildFrom(obstacleMap, true);

	m_background = new Texture("resources/textures/world_map.png");

	delete obstacleMap;

	return true;
}

void AiGame::Shutdown()
{
	
}

void AiGame::Tick()
{
	
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

			renderer->DrawSprite(m_background, 0, 0, backgroundSize.x, backgroundSize.y, 0.f, 0.f, 0.f, 0.f);
		}
	}
}