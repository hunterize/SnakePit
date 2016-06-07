#include "timing.h"


namespace SnakEngine
{
	FpsLimiter::FpsLimiter() : m_fFPS(0.0f), m_fFrameTime(0.0f), m_fTargetFPS(0.0f), m_fMaxFPS(0.0f)
	{}

	void FpsLimiter::Init(float targetFPS)
	{
		SetTargetFPS(targetFPS);
	}

	void FpsLimiter::SetTargetFPS(float targetFPS)
	{
		m_fMaxFPS = targetFPS;
	}

	void FpsLimiter::Begin()
	{
		m_uiStartTicks = SDL_GetTicks();
	}

	float FpsLimiter::End()
	{
		CalculateFPS();
		//limit the FPS to the max FPS
		float frameTicks = SDL_GetTicks() - m_uiStartTicks;

		if (m_fMaxFPS != 0.0f)
		{
			if (1000.0f / m_fMaxFPS > frameTicks)
			{
				SDL_Delay(1000.0f / m_fMaxFPS - frameTicks);
			}
			return m_fMaxFPS;
		}

		return m_fFPS;
	}

	void FpsLimiter::CalculateFPS()
	{
		static const int NUM_SAMPLES = 10;
		static float frameTimes[NUM_SAMPLES];
		static int currentFrame = 0;

		static float prevTicks = SDL_GetTicks();
		float currentTicks = SDL_GetTicks();

		m_fFrameTime = currentTicks - prevTicks;

		frameTimes[currentFrame % NUM_SAMPLES] = m_fFrameTime;

		//int ccc = (currentFrame < NUM_SAMPLES) ? currentFrame : NUM_SAMPLES;

		currentFrame++;

		int count;
		if (currentFrame < NUM_SAMPLES)
		{
			count = currentFrame;
		}
		else
		{
			count = NUM_SAMPLES;
		}




		float frameTimeAverage = 0;
		for (int i = 0; i < count; i++)
		{
			frameTimeAverage += frameTimes[i];
		}

		frameTimeAverage /= count;

		if (frameTimeAverage > 0)
		{
			m_fFPS = 1000.0f / frameTimeAverage;
		}
		else
		{
			m_fFPS = 60.0f;
		}

		prevTicks = currentTicks;



	}
}