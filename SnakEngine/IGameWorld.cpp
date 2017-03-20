#include "IGameWorld.h"


namespace SnakEngine
{
	IGameWorld::IGameWorld()
	{
		m_pScreenList = std::make_unique<CScreenList>(this);
	}


	IGameWorld::~IGameWorld()
	{
	}

	void IGameWorld::run()
	{
		if (!Init())
		{
			return;
		}

		//fixed FPS
		FpsLimiter limiter;
		limiter.SetTargetFPS(60.0f);

		m_bIsRunning = true;

		while (m_bIsRunning)
		{
			limiter.Begin();

			Update();
			Draw();

			limiter.End();

			m_cWindow.SwapBuffer();
		}
	
	}

	void IGameWorld::Exit()
	{
		m_pCurrentScreen->OnExit();

		if (m_pScreenList)
		{
			m_pScreenList->Destroy();
			m_pScreenList.reset();
		}

		m_bIsRunning = false;
	}
	
	
	void IGameWorld::Update()
	{
		if (m_pCurrentScreen)
		{
			switch (m_pCurrentScreen->GetScreenState())
			{
				case ScreenState::RUNNING :
				{
					m_pCurrentScreen->Update();
					break;
				}
				case ScreenState::CHANGING_NEXT :
				{
					m_pCurrentScreen->OnExit();
					m_pCurrentScreen = m_pScreenList->MoveToNextScreen();
					
					if (m_pCurrentScreen)
					{
						m_pCurrentScreen->SetRunning();
						m_pCurrentScreen->OnStart();
					}
					break;
				}
				case ScreenState::CHANGING_PREVIOUS :
				{
					m_pCurrentScreen->OnExit();
					m_pCurrentScreen = m_pScreenList->MoveToPreviousScreen();

					if (m_pCurrentScreen)
					{
						m_pCurrentScreen->SetRunning();
						m_pCurrentScreen->OnStart();
					}
					break;
				}
				case ScreenState::EXIT_APP :
				{
					Exit();
					break;
				}
			
				default:
					break;
			}
		}
	}

	void IGameWorld::Draw()
	{
		//glViewport(0, 0, m_cWindow.GetWidth(), m_cWindow.GetHeight());
		if (m_pCurrentScreen && m_pCurrentScreen->GetScreenState() == ScreenState::RUNNING)
		{
			m_pCurrentScreen->Draw();
		}
	}

	bool IGameWorld::Init()
	{
		SnakEngine::InitEngine();

		//SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);


		if (!InitSystem())
		{
			return false;
		}

		OnInit();

		AddScreens();

		m_pCurrentScreen = m_pScreenList->GetCurrentScreen();
		m_pCurrentScreen->OnStart();
		m_pCurrentScreen->SetRunning();

		return true;
	}

	bool IGameWorld::InitSystem()
	{
		m_cWindow.Create("Default", 1400, 600, 0);
		return true;
	}

	void IGameWorld::OnSDLEvent(SDL_Event& event)
	{
		switch (event.type)
		{
			case SDL_QUIT :
			{
				m_bIsRunning = false;
				Exit();
				break;
			}
			case SDL_KEYDOWN :
			{
				m_cInputManager.PressKey(event.key.keysym.sym);
				break;
			}
			case SDL_KEYUP :
			{
				m_cInputManager.ReleaseKey(event.key.keysym.sym);
				break;
			}
			case SDL_MOUSEBUTTONDOWN : 
			{
				m_cInputManager.PressKey(event.button.button);
				break;
			}
			case SDL_MOUSEBUTTONUP : 
			{
				m_cInputManager.ReleaseKey(event.button.button);
				break;
			}
			case SDL_MOUSEMOTION : 
			{
				m_cInputManager.SetMouseCoords((float)event.motion.x, (float)event.motion.y);
				break;
			}
			default :
				break;
		}
	}

	float IGameWorld::GetFPS()
	{
		return m_fFPS;
	}
}
