#pragma once

#include "Global.h"

class Font;
class Renderer2D;

namespace Debugging
{
	class Debugger;
}

using Debugging::Debugger;

class GameTime
{
	friend class Game;
	friend Debugger;

public:
	static float Time();
	static float DeltaTime();
	static uint FPS();

private:
	static void Init();

	static void Tick();
	static void RenderFPS(Renderer2D* renderer);

	static void Shutdown();

private:
	static float m_prevTime;
	static float m_currTime;
	static float m_deltaTime;

	static uint m_frames;
	static uint m_fps;

	static float m_fpsInterval;

	static Font* m_fpsFont;

};
