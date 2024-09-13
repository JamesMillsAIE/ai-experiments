#pragma once

#include <string>

#include <aie/bootstrap/Bootstrap2D.h>
#include <aie/bootstrap/Color.h>

using std::string;

namespace aie
{
	class Window;
	class Random;
}

DLL extern aie::Window* GetWindow();
DLL extern aie::Random* GetRandom();

namespace aie
{
	class DLL Application
	{
	public:
		friend Window* ::GetWindow();
		friend Random* ::GetRandom();

	public:
		Application() = delete;
		Application(int width, int height, const char* title, Color clrColor);
		Application(int width, int height, const char* title, Color clrColor, const string& randomSeed);
		virtual ~Application();

	public:
		// creates a window and begins the game loop which calls Tick() and Render() repeatedly
		// it first calls Startup() and if that succeeds it then starts the loop,
		// ending with Shutdown() if m_gameOver is true
		void Run();

		// show or hide the OS cursor
		void SetShowCursor(bool visible) const;

		// sets m_gameOver to true which will close the application safely when the frame ends
		void Quit() const;

	protected:
		static Application* m_instance;

	protected:
		Window* m_window;
		Random* m_random;

	protected:
		virtual void Init() = 0;

		virtual void Tick() = 0;
		virtual void Render() = 0;

		virtual void Shutdown() = 0;

	};
}