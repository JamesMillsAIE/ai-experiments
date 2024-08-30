#include "AiGame.h"

#include "AI/Locomotion/AStar.h"

#include "Core/Renderer2D.h"

using Pathfinding::Node;

AiGame::AiGame()
	: m_graph{ nullptr }
{
}

AiGame::~AiGame()
{
	delete m_graph;
}

bool AiGame::Startup()
{
	float spacing = 64.f;

	m_graph = new Graph(17, 12, spacing);
	m_graph->Build();

	return true;
}

void AiGame::Shutdown()
{
	
}

void AiGame::Tick(float deltaTime)
{
	
}

void AiGame::Render()
{
	if (Renderer2D* renderer = Renderer2D::Get())
	{
		renderer->SetRenderColour(1.f, 0.f, 0.f);

		for (uint y = 0; y < m_graph->Height(); y++)
		{
			for (uint x = 0; x < m_graph->Width(); x++)
			{
				if (Node* node = m_graph->FindNode(x, y))
				{
					renderer->DrawCircle(
						((float)x * m_graph->Spacing()) + 32.f, 
						((float)y * m_graph->Spacing()) + 32.f,
						6.f);
				}
			}
		}
	}
}