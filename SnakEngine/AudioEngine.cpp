#include "AudioEngine.h"
#include "Errors.h"

namespace SnakEngine
{

	void SoundEffect::Play(int loops)
	{
		if (Mix_PlayChannel(-1, m_pChunk, loops) == -1)
		{
			//if (Mix_PlayChannel(0, m_pChunk, loops) == -1)
			{
				fatalError("Mix_PlayChannel Error: " + std::string(Mix_GetError()));
			}
		}
	}


	void Music::Play(int loops)
	{
		Mix_PlayMusic(m_pMusic, loops);
	}

	void Music::Pause()
	{
		Mix_PauseMusic();
	}

	void Music::Stop()
	{
		Mix_HaltMusic();
	}

	void Music::Resume()
	{
		Mix_ResumeMusic();
	}


	AudioEngine::AudioEngine()
	{
	}


	AudioEngine::~AudioEngine()
	{
		Destroy();
	}

	void AudioEngine::Init()
	{
		if (m_bIsInitialized)
		{
			fatalError("Audio Engine is already initialized \n");
		}

		if (Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG) == -1)
		{
			fatalError("Mix_Init Error: " + std::string(Mix_GetError()));
		}

		if (Mix_OpenAudio(MIX_DEFAULT_FREQUENCY, MIX_DEFAULT_FORMAT, 2, 1024) == -1)
		{
			fatalError("Mix_OpenAudio Error: " + std::string(Mix_GetError()));
		}


		m_bIsInitialized = true;
	}

	void AudioEngine::Destroy()
	{
		if(m_bIsInitialized)
		{
			m_bIsInitialized = false;
			
			for (auto& it : m_effectMap)
			{
				Mix_FreeChunk(it.second);
			}

			for (auto& it : m_musicMap)
			{
				Mix_FreeMusic(it.second);
			}

			m_effectMap.clear();
			m_musicMap.clear();

			Mix_CloseAudio();
			Mix_Quit();
		}
	}

	SoundEffect AudioEngine::LoadSoundEffect(const std::string& filePath)
	{
		auto it = m_effectMap.find(filePath);
		
		SoundEffect effect;

		if (it == m_effectMap.end())
		{
			Mix_Chunk* pChunk = Mix_LoadWAV(filePath.c_str());

			if (pChunk == nullptr)
			{
				fatalError("Mix_LoadWAV Error: " + std::string(Mix_GetError()));
			}

			effect.m_pChunk = pChunk;
			m_effectMap[filePath] = pChunk;
		}
		else
		{
			effect.m_pChunk = it->second;
		}

		return effect;

	}

	Music AudioEngine::LoadMusic(const std::string& filePath)
	{
		Music music;

		auto it = m_musicMap.find(filePath);

		if(it == m_musicMap.end())
		{
			Mix_Music* pMusic = Mix_LoadMUS(filePath.c_str());

			if (pMusic == nullptr)
			{
				fatalError("Mix_LoadMUS Error: " + std::string(Mix_GetError()));
			}

			music.m_pMusic = pMusic;
			m_musicMap[filePath] = pMusic;
		}
		else
		{
			music.m_pMusic = it->second;
		}


		return music;
		
		
	}

}