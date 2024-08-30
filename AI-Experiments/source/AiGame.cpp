#include "AiGame.h"

#include <string>

#include <Core/Font.h>
#include <Core/Renderer2D.h>

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