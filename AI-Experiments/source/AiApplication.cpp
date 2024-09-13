#include "AiApplication.h"

#include "Debugging/Debugger.h"
#include "Debugging/ImGuiAdapter.h"

#include <aie/bootstrap/Renderer.h>

using Debugging::Debugger;

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