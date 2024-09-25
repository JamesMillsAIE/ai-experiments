#include "AiApplication.h"

#include <aie/bootstrap/Renderer.h>

#include "Global.h"

#include "Actors/ActorManager.h"

#include "AI/Agent.h"

#include "Debugging/Debugger.h"
#include "Debugging/ImGuiAdapter.h"

#include "Levels/LevelManager.h"
#include "Levels/MainLevel.h"

using Debugging::Debugger;

ImGuiAdapter* GetImGuiAdapter()
{
	return AiApplication::GetInstance()->m_imGui;
}

ActorManager* GetActorManager()
{
	return AiApplication::GetInstance()->m_actorManager;
}

LevelManager* GetLevelManager()
{
	return AiApplication::GetInstance()->m_levelManager;
}

AiApplication::AiApplication()
	: Application{ 912, 840, "AI Playground", { 255, 255, 255, 255 } }, m_imGui{ new ImGuiAdapter },
	m_actorManager{ new ActorManager }, m_levelManager{ new LevelManager }
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

	m_levelManager->AddLevel(new MainLevel);
	m_levelManager->Open(MAIN_LEVEL);

	Debugger::Create(m_window);
}

void AiApplication::Tick()
{
	m_imGui->ImGuiNewFrame();

	m_levelManager->Tick();
	m_actorManager->Tick();

	if (Debugger* debugger = Debugger::m_instance)
	{
		debugger->Tick();
	}
}

void AiApplication::Render()
{
	m_levelManager->Render(Renderer::Get());
	m_actorManager->Render(Renderer::Get());

	if (Debugger* debugger = Debugger::m_instance)
	{
		debugger->Render(Renderer::Get());
	}

	m_imGui->ImGuiRender();
}

void AiApplication::Shutdown()
{
	Debugger::Destroy();

	delete m_levelManager;
	delete m_actorManager;

	m_imGui->ShutdownImGui();
}