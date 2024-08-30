#include "Game.h"

#include <GL/glew.h>

#include <glfw/glfw3.h>

#include <glm/glm.hpp>

#include "Input.h"
#include "Random.h"
#include "Window.h"

#include "Agents/AgentManager.h"

Game::Game()
	: m_window{ nullptr }, m_random{ new Random }, m_fps{ 0 }
{
	m_agentManager = new AgentManager(m_random);
}

Game::~Game()
{
	delete m_agentManager;
	delete m_window;
	delete m_random;
}

void Game::Run(const char* title, const int width, const int height, const bool fullscreen)
{
	m_window = new Window(width, height, title, { 1.f, 1.f, 1.f, 1.f });

	// start game loop if successfully initialised
	if (m_window->Open() && Startup())
	{
		m_random->Seed("");

		Input::Create();

		// variables for timing
		double prevTime = glfwGetTime();
		unsigned int frames = 0;
		double fpsInterval = 0;

		// loop while game is running
		while (m_window->IsOpen())
		{
			// Tick delta time
			const double currTime = glfwGetTime();
			double deltaTime = currTime - prevTime;
			if (deltaTime > 0.1f)
			{
				deltaTime = 0.1f;
			}

			prevTime = currTime;

			// clear input
			Input::Get()->ClearStatus();

			// Tick window events (input, etc.)
			glfwPollEvents();

			// skip if minimised
			if (glfwGetWindowAttrib(*m_window, GLFW_ICONIFIED) != 0)
				continue;

			// Tick fps every second
			frames++;
			fpsInterval += deltaTime;
			if (fpsInterval >= 1.0f)
			{
				m_fps = frames;
				frames = 0;
				fpsInterval -= 1.0f;
			}

			Tick(static_cast<float>(deltaTime));
			m_agentManager->Tick(static_cast<float>(deltaTime));

			m_window->NewFrame();

			m_agentManager->Render();
			Render();

			m_window->EndFrame();
		}

		Input::Destroy();
		m_window->Close();
	}

	// cleanup
	Shutdown();
}

void Game::Quit() const
{
	m_window->m_isOpen = false;
}

void Game::SetShowCursor(const bool visible) const
{
	glfwSetInputMode(*m_window, GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN);
}

float Game::GetTime() const
{
	return static_cast<float>(glfwGetTime());
}