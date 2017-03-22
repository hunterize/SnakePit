#pragma once

#include "SnakEngine.h"
#include "SWindow.h"
#include "IGameScreen.h"
#include "ScreenList.h"
#include "Timing.h"
#include "InputManager.h"

#include <memory>

namespace SnakEngine
{
	class IGameWorld
	{
	public:
		IGameWorld();
		virtual ~IGameWorld();

		void run();

		void Exit();

		virtual void OnInit() = 0;
		virtual void OnExit() = 0;

		virtual void AddScreens() = 0;

		void OnSDLEvent(SDL_Event& event);

		float GetFPS();
		
		InputManager& GetInputManager() { return m_cInputManager; }

	protected:
		virtual void Update();
		virtual void Draw();

		bool Init();
		bool InitSystem();

		std::unique_ptr<CScreenList> m_pScreenList = nullptr;
		IGameScreen* m_pCurrentScreen = nullptr;
		bool m_bIsRunning = false;
		float m_fFPS = 0.0f;
		
		SWindow m_cWindow;
		InputManager m_cInputManager;
	};
}

