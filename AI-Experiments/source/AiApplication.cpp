#include "AiApplication.h"

#include <numbers>

#include <aie/bootstrap/Renderer.h>
#include <aie/bootstrap/Window.h>

#include "AI/Pathfinding/NavMesh/NavigationMesh.h"
#include "AI/Pathfinding/NavMesh/NavigationObstacle.h"

#include "Debugging/Debugger.h"
#include "Debugging/ImGuiAdapter.h"

using Debugging::Debugger;

using Pathfinding::NavigationMesh;
using Pathfinding::NavigationObstacle;
using Pathfinding::EObstacleBuildFlags;

NavigationMesh* navigationMesh = nullptr;

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
}

AiApplication* AiApplication::GetInstance()
{
	return dynamic_cast<AiApplication*>(m_instance);
}

void AiApplication::Init()
{
	m_imGui->InitImGui();

	Debugger::Create(m_window);

	navigationMesh = new NavigationMesh(m_window->GetWidth(), m_window->GetHeight());

	NavigationObstacle* obstacle = new NavigationObstacle(10.f);

	obstacle->AddVertex({ 200, 200 });
	obstacle->AddVertex({ 200, 400 });
	obstacle->AddVertex({ 400, 400 });
	obstacle->AddVertex({ 400, 800 });
	obstacle->AddVertex({ 800, 400 });

	NavigationObstacle* obstacleBuilt = new NavigationObstacle(10.f);

	obstacleBuilt->AddVertex({ 200, 200 });
	obstacleBuilt->AddVertex({ 200, 400 });
	obstacleBuilt->AddVertex({ 400, 400 });
	obstacleBuilt->AddVertex({ 400, 800 });
	obstacleBuilt->AddVertex({ 800, 400 });

	obstacleBuilt->Build(EObstacleBuildFlags::GenerateConvexHull | EObstacleBuildFlags::AddPaddingToHull);

	if(!navigationMesh->AddObstacle(obstacle))
	{
		delete obstacle;
	}

	if(!navigationMesh->AddObstacle(obstacleBuilt))
	{
		delete obstacleBuilt;
	}

	navigationMesh->Build();
}

void AiApplication::Tick()
{
	m_imGui->ImGuiNewFrame();

	if (Debugger* debugger = Debugger::m_instance)
	{
		debugger->Tick();
	}
}

void AiApplication::Render()
{
	if (Debugger* debugger = Debugger::m_instance)
	{
		debugger->Render(Renderer::Get());
	}

	m_imGui->ImGuiRender();

	
}

void AiApplication::Shutdown()
{
	Debugger::Destroy();

	m_imGui->ShutdownImGui();
}