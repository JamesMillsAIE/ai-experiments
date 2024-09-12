#pragma once

#include <string>

#include <aie/bootstrap/Bootstrap2D.h>

#include <fmod/fmod.hpp>

using std::string;

namespace aie
{
	class DLL SoundClip
	{
		friend class Audio;

	private:
		FMOD::Sound* m_soundHandle;
		FMOD::Channel* m_channel;

		string m_path;

	private:
		explicit SoundClip(string path);
		~SoundClip();

	private:
		bool Load(FMOD::System* system);

	};
}
