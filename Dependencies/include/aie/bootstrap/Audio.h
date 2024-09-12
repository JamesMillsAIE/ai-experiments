#pragma once

#include <string>

#include <aie/bootstrap/Bootstrap2D.h>

#include <fmod/fmod.hpp>

using std::string;

namespace aie
{
	class SoundClip;

	class DLL Audio
	{
		friend class Application;

	public:
		static Audio* Get();

	public:
		SoundClip* Load(const string& path) const;
		void Play(SoundClip* sound) const;

	private:
		static Audio* m_instance;

	private:
		static void Create();
		static void Destroy();

		static bool Init();
		static void Shutdown();

	private:
		FMOD::System* m_system;

	private:
		Audio();
		~Audio();

	};
}

