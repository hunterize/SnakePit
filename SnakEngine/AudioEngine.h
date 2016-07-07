#pragma once
#include <Mixer\SDL_mixer.h>
#include <string>
#include <map>

namespace SnakEngine
{

	class SoundEffect
	{
	public:
		friend class AudioEngine;

		void Play(int loops = 0);

	private:
		Mix_Chunk* m_pChunk = nullptr;
	};

	class Music
	{
	public:
		friend class AudioEngine;

		void Play(int loops = 1);

		static void Pause();
		static void Stop();
		static void Resume();

	private:
		Mix_Music* m_pMusic = nullptr;

	};
	
	
	
	class AudioEngine
	{
	public:
		AudioEngine();
		~AudioEngine();

		void Init();
		void Destroy();

		SoundEffect LoadSoundEffect(const std::string& filePath);
		Music LoadMusic(const std::string& filePath);

	private:
		std::map<std::string, Mix_Chunk*> m_effectMap;
		std::map<std::string, Mix_Music*> m_musicMap;

		bool m_bIsInitialized = false;
	};
}

