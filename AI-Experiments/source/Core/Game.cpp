#include "Game.h"

#include <Core/Debugging/Debugger.h>
#include <Core/Rendering/Renderer2D.h>
#include <Core/Structures/Random.h>

#include <GL/glew.h>

#include <glfw/glfw3.h>

#include <glm/glm.hpp>

#include "GameTime.h"
#include "Input.h"
#include "Window.h"

using Debugging::Debugger;

Game::Game()
	: m_window{ nullptr }, m_random{ new Random }
{
}

Game::~Game()
{
	delete m_window;
	delete m_random;
}

void Game::Run(const char* title, const int width, const int height, const bool fullscreen)
{
	m_window = new Window(width, height, title, { 1.f, 1.f, 1.f, 1.f });

	Debugger::Create(m_window);

	m_random->Seed("");

	// start game loop if successfully initialised
	if (m_window->Open() && Startup())
	{
		Input::Create();
		GameTime::Init();

		m_window->InitImGui();

		// loop while game is running
		while (m_window->IsOpen())
		{
			// clear input
			Input::Get()->ClearStatus();

			// Tick window events (input, etc.)
			glfwPollEvents();

			// skip if minimised
			if (glfwGetWindowAttrib(*m_window, GLFW_ICONIFIED) != 0)
			{
				continue;
			}

			GameTime::Tick();

			Tick();

			m_window->NewFrame();

			if (Debugger* debugger = Debugger::m_instance)
			{
				debugger->Tick();
				debugger->Render(Renderer2D::Get());
			}

			Render();

			m_window->EndFrame();
		}

		GameTime::Shutdown();
		Input::Destroy();
		m_window->Close();
	}

	// cleanup
	Shutdown();

	Debugger::Destroy();
}

void Game::Quit() const
{
	m_window->m_isOpen = false;
}

void Game::SetShowCursor(const bool visible) const
{
	glfwSetInputMode(*m_window, GLFW_CURSOR, visible ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_HIDDEN);
}