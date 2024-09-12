#pragma once

#include <aie/bootstrap/Bootstrap2D.h>

namespace aie
{
	class DLL GameTime
	{
		friend class Application;

	public:
		static float Time();
		static float DeltaTime();
		static unsigned int FPS();

	private:
		static void Init();

		static void Tick();

	private:
		static float m_prevTime;
		static float m_currTime;
		static float m_deltaTime;

		static unsigned int m_frames;
		static unsigned int m_fps;

		static float m_fpsInterval;
	};
}
