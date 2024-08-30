#pragma once

class Window;

class Game
{
public:
	Game();
	virtual ~Game();

	// creates a window and begins the game loop which calls Tick() and Render() repeatedly
	// it first calls Startup() and if that succeeds it then starts the loop,
	// ending with Shutdown() if m_gameOver is true
	void Run(const char* title, int width, int height, bool fullscreen);

	// these functions must be implemented by a derived class
	virtual bool Startup() = 0;
	virtual void Shutdown() = 0;
	virtual void Tick(float deltaTime) = 0;
	virtual void Render() = 0;

	// show or hide the OS cursor
	void SetShowCursor(bool visible) const;

	// sets m_gameOver to true which will close the application safely when the frame ends
	void Quit() const;

	// returns the frames-per-second that the loop is running at
	unsigned int GetFPS() const { return m_fps; }

	// returns time since application started
	float GetTime() const;

protected:
	Window* m_window;

	unsigned int m_fps;

};