#include "AiApplication.h"

#include <iostream>

#include <aie/bootstrap/Input.h>
#include <aie/bootstrap/Renderer.h>
#include <aie/bootstrap/Window.h>

#include <glm/geometric.hpp>

#include "Global.h"

#include "AI/Pathfinding/Node.h"
#include "AI/Pathfinding/Algorithms/AStar.h"
#include "AI/Pathfinding/NavMesh/NavigationMesh.h"

#include "Debugging/Debugger.h"
#include "Debugging/ImGuiAdapter.h"

using Debugging::Debugger;

using Pathfinding::Node;
using Pathfinding::NavigationMesh;
using Pathfinding::AStar;

using aie::Input;

NavigationMesh* navMesh;
bool firstClick = false;
vec2 firstPos;

vector<Node*> path;

float Heuristic(Node* a, Node* b)
{
	return glm::distance(a->position, b->position);
}

ImGuiAdapter* GetImGuiAdapter()
{
	return AiApplication::GetInstance()->m_imGui;
}

AiApplication::AiApplication()
	: Application{ 912, 840, "AI Playground", { 255, 255, 255, 255 } }, m_imGui{ new ImGuiAdapter }
{
}

AiApplication::~AiApplication()
{
	delete m_imGui;
	delete navMesh;
}

AiApplication* AiApplication::GetInstance()
{
	return dynamic_cast<AiApplication*>(m_instance);
}

void AiApplication::Init()
{
	m_imGui->InitImGui();

	Debugger::Create(m_window);

	navMesh = new NavigationMesh(m_window->GetWidth(), m_window->GetHeight());
	navMesh->AddRandomObstacles(12, 60, 60, m_random);

	navMesh->Build();
}

void AiApplication::Tick()
{
	m_imGui->ImGuiNewFrame();

	if (Debugger* debugger = Debugger::m_instance)
	{
		debugger->Tick();
	}

	if (Input* input = Input::Get())
	{
		if (input->WasMouseButtonReleased(aie::EInputCodes::MouseButtonLeft))
		{
			if (firstClick)
			{
				firstPos = AieToGlm(input->GetMousePos());
			}
			else
			{
				if(AStar::FindPath(firstPos, AieToGlm(input->GetMousePos()), navMesh, &Heuristic, path))
				{
					std::cout << "Found path!\n";
					path = navMesh->SmoothPath(path);
				}
				else
				{
					std::cout << "Failed to find path!\n";
				}
			}

			firstClick = !firstClick;
		}
	}
}

void AiApplication::Render()
{
	if (Debugger* debugger = Debugger::m_instance)
	{
		debugger->Render(Renderer::Get());
	}

	m_imGui->ImGuiRender();

	if (Renderer* renderer = Renderer::Get())
	{
		if (!path.empty())
		{
			for(size_t i = 1; i < path.size(); ++i)
			{
				Vector2 start = GlmToAie(path[i]->position);
				Vector2 end = GlmToAie(path[i - 1]->position);

				renderer->DrawLine(start, end, 2.f, { 255, 255, 0, 255 });
			}
		}
	}
}

void AiApplication::Shutdown()
{
	Debugger::Destroy();

	m_imGui->ShutdownImGui();
}