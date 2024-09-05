#include "GameTime.h"

#include <string>

#include <GLFW/glfw3.h>

#include "Debugger.h"
#include "Font.h"
#include "Renderer2D.h"

using Debugging::Debugger;

float GameTime::m_prevTime;
float GameTime::m_currTime;
float GameTime::m_deltaTime;

uint GameTime::m_frames;
uint GameTime::m_fps;

float GameTime::m_fpsInterval;

Font* GameTime::m_fpsFont = nullptr;

float GameTime::Time()
{
	return m_currTime;
}

float GameTime::DeltaTime()
{
	return m_deltaTime;
}

uint GameTime::FPS()
{
	return m_fps;
}

void GameTime::Init()
{
	m_currTime = 0;
	m_prevTime = static_cast<float>(glfwGetTime());
	m_fpsInterval = 0.f;

	m_fpsFont = new Font("resources/font/consolas.ttf", 25);
}

void GameTime::Tick()
{
	m_currTime = static_cast<float>(glfwGetTime());
	m_deltaTime = m_currTime - m_prevTime;
	m_prevTime = m_currTime;

	m_frames++;
	m_fpsInterval += m_deltaTime;

	if (m_fpsInterval >= 1.f)
	{
		m_fps = m_frames;
		m_frames = 0;
		m_fpsInterval -= 1.f;
	}
}

void GameTime::RenderFPS(Renderer2D* renderer)
{
	renderer->SetRenderColour(0.f, 0.f, 0.f);
	renderer->DrawText(m_fpsFont, std::to_string(m_fps).c_str(), { 10, 10 });
}

void GameTime::Shutdown()
{
	delete m_fpsFont;
}
