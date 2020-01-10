
#include "IGameScreen.h"

namespace SnakEngine
{

	IGameScreen::IGameScreen()
	{
	}


	IGameScreen::~IGameScreen()
	{
	}

	int IGameScreen::GetScreenIndex()
	{
		return m_iIndex;
	}

	ScreenState IGameScreen::GetScreenState()
	{
		return m_eState;
	}

	void IGameScreen::SetParentGame(IGameWorld* pGame)
	{
		m_pGame = pGame;
	}

	void IGameScreen::SetRunning()
	{
		m_eState = ScreenState::RUNNING;
	}
}