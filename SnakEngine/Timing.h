#pragma once
#include <SDL\SDL.h>


namespace SnakEngine
{
	class FpsLimiter
	{
	public:
		FpsLimiter();

		void SetTargetFPS(float targetFPS);
		void Init(float targetFPS);
		void Begin();

		float End();
		float GetFrameTime() { return m_fFrameTime; }

		void CalculateFPS();

	private:
		float m_fTargetFPS;

		//game time
		float m_fFrameTime;
		//FPS
		float m_fFPS;
		//Max FPS
		float m_fMaxFPS;
		unsigned int m_uiStartTicks;
	};
}