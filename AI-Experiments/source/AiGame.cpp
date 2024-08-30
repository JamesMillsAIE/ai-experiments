#include "AiGame.h"

#include <string>

#include <Core/Font.h>

#include "Agents/Agent.h"
#include "Agents/AgentManager.h"

using std::string;

AiGame::AiGame()
{
	m_font = nullptr;
}

AiGame::~AiGame()
{
	delete m_font;
}

bool AiGame::Startup()
{
	m_font = new Font("resources/font/consolas.ttf", 20);

	for (size_t i = 0; i < 50; i++)
	{
		m_agentManager->Spawn(new Agent);
	}

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
}