#include "AiApplication.h"

#include <aie/bootstrap/Renderer.h>

#include "Actors/ActorManager.h"

#include "AI/Agent.h"

#include "Debugging/Debugger.h"
#include "Debugging/ImGuiAdapter.h"

using Debugging::Debugger;

ImGuiAdapter* GetImGuiAdapter()
{
	return AiApplication::GetInstance()->m_imGui;
}

ActorManager* GetActorManager()
{
	return AiApplication::GetInstance()->m_actorManager;
}

AiApplication::AiApplication()
	: Application{ 912, 840, "AI Playground", { 255, 255, 255, 255 } }, m_imGui{ new ImGuiAdapter },
	m_actorManager{ new ActorManager }
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
}

void AiApplication::Tick()
{
	m_imGui->ImGuiNewFrame();

	m_actorManager->Tick();

	if (Debugger* debugger = Debugger::m_instance)
	{
		debugger->Tick();
	}
}

void AiApplication::Render()
{
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

	delete m_actorManager;

	m_imGui->ShutdownImGui();
}