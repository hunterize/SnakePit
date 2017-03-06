#pragma once
//#include "IGameScreen.h"
#include <vector>

namespace SnakEngine
{

	class IGameWorld;
	class IGameScreen;

	class CScreenList
	{
	public:
		CScreenList(IGameWorld* pGame);
		~CScreenList();

		void AddScreen(IGameScreen* pScreen);
		void SetScreen(int iIndex);

		IGameScreen* MoveToNextScreen();
		IGameScreen* MoveToPreviousScreen();
		IGameScreen* GetCurrentScreen();

		void Destroy();

	private:
		std::vector<IGameScreen*> m_cScreenList;
		IGameWorld* m_pMainGame = nullptr;
		int m_iCurrentScreenIndex = -1;
	};
}
