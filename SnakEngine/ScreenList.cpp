#include "ScreenList.h"


namespace SnakEngine
{
	CScreenList::CScreenList(IGameWorld* pGame) : m_pMainGame(pGame)
	{
	}


	CScreenList::~CScreenList()
	{
	}

	void CScreenList::AddScreen(IGameScreen* pScreen)
	{
		pScreen->m_iIndex = m_cScreenList.size();
		m_cScreenList.push_back(pScreen);

		pScreen->Create();
		pScreen->SetParentGame(m_pMainGame);
	}

	IGameScreen* CScreenList::MoveToNextScreen()
	{
		IGameScreen* pScreen = GetCurrentScreen();
		if (pScreen != nullptr)
		{
			if (pScreen->GetNextScreenIndex() != -1)
			{
				m_iCurrentScreenIndex = pScreen->GetNextScreenIndex();
			}

			return GetCurrentScreen();
		}
		else
		{
			return nullptr;
		}

	}

	IGameScreen* CScreenList::MoveToPreviousScreen()
	{
		IGameScreen* pScreen = GetCurrentScreen();
		if (pScreen != nullptr)
		{
			if (pScreen->GetPreviousScreenIndex() != -1)
			{
				m_iCurrentScreenIndex = pScreen->GetPreviousScreenIndex();
			}
			return GetCurrentScreen();
		}
		else
		{
			return nullptr;
		}
	}

	IGameScreen* CScreenList::GetCurrentScreen()
	{
		if (m_iCurrentScreenIndex == -1)
		{
			return nullptr;
		}
		
		return m_cScreenList[m_iCurrentScreenIndex];
	}

	void CScreenList::Destroy()
	{
		for (size_t i = 0; i < m_cScreenList.size(); i++)
		{
			m_cScreenList[i]->Destroy();
		}

		m_cScreenList.resize(0);
		m_iCurrentScreenIndex = -1;
	}
}