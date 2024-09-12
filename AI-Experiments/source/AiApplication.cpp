#include "AiApplication.h"

#include "Debugging/ImGuiAdapter.h"

AiApplication::AiApplication()
	: Application{ 912, 840, "AI Playground", { 255, 255, 255, 255 } }, m_imGui{ new ImGuiAdapter }
{
}

void AiApplication::Init()
{
	m_imGui->InitImGui();
}

void AiApplication::Tick()
{
	m_imGui->ImGuiNewFrame();
}

void AiApplication::Render()
{
	m_imGui->ImGuiRender();
}

void AiApplication::Shutdown()
{
	m_imGui->ShutdownImGui();
}